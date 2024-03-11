OPTION="-b --configuration json://config.json"
o2-analysis-track-propagation ${OPTION} | 
o2-analysis-timestamp ${OPTION} | 
o2-analysis-pid-tpc ${OPTION} |
o2-analysis-pid-tpc-base ${OPTION} |
o2-analysis-event-selection ${OPTION} |
o2-analysis-trackselection ${OPTION} |
o2-analysis-lf-lambdakzerobuilder ${OPTION} |
o2-analysis-lf-lambdakzeromcbuilder ${OPTION} | 
o2-analysis-bc-converter ${OPTION} |
o2-analysis-zdc-converter ${OPTION} |
o2-analysis-v0converter ${OPTION} |
o2-analysis-tracks-extra-converter ${OPTION} |
o2-analysistutorial-mm-my-example-task ${OPTION}