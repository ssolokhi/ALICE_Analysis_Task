// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
///
/// \brief example of Run3 data analysis
/// \author Sergei Solokhin
/// \since 20/09/2023

#include "Framework/AnalysisTask.h" // needed in any case
#include "Framework/runDataProcessing.h" // needed in any case
#include "Framework/ASoA.h" // For columns and tables
#include "Framework/AnalysisDataModel.h" // For extending the standard AOD format
#include "Framework/ASoAHelpers.h" // For Filters

#include "Common/DataModel/EventSelection.h"
#include "Common/DataModel/TrackSelectionTables.h" // For DCA info
#include "Common/DataModel/PIDResponse.h" // For PID: expected values, resolutions,etc.

#include "PWGEM/PhotonMeson/DataModel/gammaTables.h" // For PHOS clusters
#include "PWGLF/DataModel/LFStrangenessTables.h" // for strangeness analysis
#include "CommonConstants/PhysicsConstants.h" // for masses

#include "TLorentzVector.h"
#include <cmath>

using namespace o2::aod;
using namespace o2::soa;
using namespace o2::framework;
using namespace o2::framework::expressions; // for filters

struct AnalysisExample {
  float chargedPionMass = o2::constants::physics::MassPionCharged;

  // General:
  Configurable<int> nBinsPt{"nBinsPt", 100, "N bins in pT histo"};
  Configurable<int> nBinsMass{"nBinsMass", 100, "N bins in invariant mass histo"};
  Configurable<float> etaCut{"etaCut", 1.2f, "Maximum Pseudorapidity"};

  // Collisions-related:
  Configurable<float> zVertexCut{"zVertexCut", 10.0f, "Maximum Primary Vertex Z coordinate [cm]"};
  Filter zVertexFilter = nabs(collision::posZ) < zVertexCut && collision::posZ != 0.0f;
  Filter eventSelectionFilter = evsel::sel8 == true;
  using filteredCollision = Filtered<Join<Collisions, EvSels>>::iterator;

  // Decay vertex-related:
  Configurable<float> v0setting_dcav0dau{"v0setting_dcav0dau", 1.0f, "DCA V0 Daughters [cm]"};
  Configurable<float> v0setting_dcapostopv{"v0setting_dcapostopv", 0.06f, "DCA Pos To PV [cm]"};
  Configurable<float> v0setting_dcanegtopv{"v0setting_dcanegtopv", 0.06f, "DCA Neg To PV [cm]"};
  Configurable<double> v0setting_cospa{"v0setting_cospa", 0.98, "V0 CosPA"};
  Configurable<float> v0setting_radius{"v0setting_radius", 0.5f, "V0 Radius [cm]"};

  Configurable<float> maxQt{"maxQt", 0.5f, "Maximum Decay Product Transverse Momentum [GeV]"};

  // filter can only be applied to static columns
  Filter preV0Filter = nabs(v0data::dcapostopv) > v0setting_dcapostopv
  && nabs(v0data::dcanegtopv) > v0setting_dcanegtopv 
  && v0data::dcaV0daughters < v0setting_dcav0dau;
  using filteredV0s = Filtered<Join<V0Datas, McV0Labels>>;

  // Tracks-related:
  Configurable<float> maxTpcNSigmaPi{"maxTpcNSigmaPi", 3.0f, "Maximum N_{#sigma_{#pi}} from TPC signal"};
  Configurable<float> maxTrackDCA{"maxTrackDCA", 0.5f, "Maximum Track DCA [cm]"};

  Filter etaFilter = nabs(track::eta) < etaCut;
  Filter dcaFilter = nabs(track::dcaXY) < maxTrackDCA;

  using daughterTracks = Join<Tracks, TracksExtra, TracksDCA, pidTPCPi, McTrackLabels>;
  using filteredTracks = Filtered<daughterTracks>;

  HistogramRegistry histosCollisions{"histosCollisions", {}, OutputObjHandlingPolicy::AnalysisObject};
  HistogramRegistry histosDecayVertex{"histosDecayVertex", {}, OutputObjHandlingPolicy::AnalysisObject};
  HistogramRegistry histosTracks{"histosTracks", {}, OutputObjHandlingPolicy::AnalysisObject};
  HistogramRegistry histosMC{"histosMC", {}, OutputObjHandlingPolicy::AnalysisObject};

  void init(InitContext const&) {
    printf("Init\n");
    // define axes:
    const AxisSpec axisVertexZ{100, -10.0f, 10.0f, "Vertex Z Coordinate [cm]"};
    const AxisSpec axisPt{nBinsPt, 0, 10.0f, "p_{T} [GeV]"};
    const AxisSpec axisQt{nBinsPt, 0, 0.5f, "q_{T} [GeV]"};
    const AxisSpec axisEta{300, -1.5f, 1.5f, "#eta"};
    const AxisSpec axisTheta{100, 0, M_PI, "#theta"};
    const AxisSpec axisPhi{300, 0, 2*M_PI, "#phi"};

    // General collision histograms:
    histosCollisions.add("vertexZ", "vertexZ", kTH1F, {axisVertexZ});
    histosCollisions.add("eventCounter", "eventCounter", kTH1F, {{1, 0, 1, "Accepted Events Count"}});
    histosCollisions.add("nContributors", "nContributors", kTH1F, {{100, 0, 100, "Number of Collision Contributors"}});

    // Monte-Carlo histograms:
    histosMC.add("mcV0Energy", "mcV0Energy", kTH1F, {{100, 0, 10, "E_{gen} {GeV}"}});

    // Decay Vertex histograms:
    histosDecayVertex.add("ArmenterosPodolanskiPlot", "ArmenterosPodolanskiPlot", kTH2F, {{100, -1, 1, "#alpha"}, axisQt});
    histosDecayVertex.add("v0PosNsigmaPi", "v0PosNsigmaPi", kTH1F, {{100, -5, 5, "N_{#sigma, #pi}"}});
    histosDecayVertex.add("v0NegNsigmaPi", "v0NegNsigmaPi", kTH1F, {{100, -5, 5, "N_{#sigma, #pi}"}});

    // Track histograms:
    histosTracks.add("etaVsPhi", "etaVsPhi", kTH2F, {axisEta, axisPhi});
    histosTracks.add("TPCresponse", "TPCresponse", kTH2F, {axisPt, {500, 0, 500, "TPC Response [a.u.]"}});
    histosTracks.add("pionResponse", "pionResponse", kTH2F, {axisPt, {500, 0, 500, "TPC Pion Response [a.u.]"}});
    histosTracks.add("TRDresponse", "TRDresponse", kTH2F, {axisPt, {1000, -1000, 0, "TRD Response [a.u.]"}});
    histosTracks.add("tpcNClsCrossed", "tpcNClsCrossed", kTH1F, {{160, 0, 160, "Number of Crossed TPC Clusters"}});
  }

  void processCollisions(filteredCollision const& collision, filteredTracks const& tracks) {
    printf("Process collisions\n");
    histosCollisions.fill(HIST("vertexZ"), collision.posZ());
    histosCollisions.fill(HIST("eventCounter"), 0.5);
    histosCollisions.fill(HIST("nContributors"), collision.numContrib());
    for (auto const& track: tracks) {
      printf("Process track within collision\n");
      histosTracks.fill(HIST("etaVsPhi"), track.eta(), track.phi());
      histosTracks.fill(HIST("TPCresponse"), track.p(), track.tpcSignal());
      histosTracks.fill(HIST("TRDresponse"), track.p(), track.trdSignal());
      histosTracks.fill(HIST("tpcNClsCrossed"), track.tpcNClsCrossedRows());
      if (abs(track.tpcNSigmaPi()) < 3.0f) histosTracks.fill(HIST("pionResponse"), track.p(), track.tpcSignal());
    }
  }
  PROCESS_SWITCH(AnalysisExample, processCollisions, "Process general collisions info", true);

void processVerteces(filteredCollision const& collision, filteredV0s const& verteces, filteredTracks const&, McParticles const&) {
    printf("Process Decay Verteces\n");
    for (auto const& vertex: verteces) {
      // "Filter" on dynamic columns
      if (vertex.v0cosPA() < v0setting_cospa) continue;
      if (vertex.v0radius() < v0setting_radius) continue;

      auto const& posDaughterTrack = vertex.posTrack_as<filteredTracks>();
      auto const& negDaughterTrack = vertex.negTrack_as<filteredTracks>();

      float const tpcNposSigmaPi = posDaughterTrack.tpcNSigmaPi();
      float const tpcNnegSigmaPi = negDaughterTrack.tpcNSigmaPi();
      histosDecayVertex.fill(HIST("v0PosNsigmaPi"), tpcNposSigmaPi);
      histosDecayVertex.fill(HIST("v0NegNsigmaPi"), tpcNnegSigmaPi);
      histosDecayVertex.fill(HIST("ArmenterosPodolanskiPlot"), vertex.alpha(), vertex.qtarm());

      if (vertex.has_mcParticle()) {
        auto const& v0mcParticle = vertex.mcParticle();
              histosMC.fill(HIST("mcV0Energy"), v0mcParticle.e());
      }
    }   
  }
  PROCESS_SWITCH(AnalysisExample, processVerteces, "Process Decay Verteces", true);
};

WorkflowSpec defineDataProcessing(ConfigContext const& cfgc) {
  return WorkflowSpec{
    adaptAnalysisTask<AnalysisExample>(cfgc)
  };
}