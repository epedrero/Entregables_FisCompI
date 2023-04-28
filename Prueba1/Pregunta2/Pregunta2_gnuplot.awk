awk '$1=="ATOM" && $12=="C"{print $11}' 1ubq.pdb > C_beta.csv
awk '$1=="ATOM" && $12=="N"{print $11}' 1ubq.pdb > N_beta.csv
awk '$1=="ATOM" && $12=="O"{print $11}' 1ubq.pdb > O_beta.csv
awk '$1=="ATOM" && $12=="S"{print $11}' 1ubq.pdb > S_beta.csv

