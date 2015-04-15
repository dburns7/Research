#!/bin/sh

rm ../results/histos.root

python scripts/cluster_analysis.py --dev ce516 --runfile ../data/ce516.root --hotpix 1

python scripts/cluster_analysis.py --dev 26019 --runfile ../data/26019.root --hotpix 1

python scripts/cluster_analysis.py --dev 2858f --runfile ../data/2858f.root --hotpix 1

python scripts/cluster_analysis.py --dev 80dce --runfile ../data/80dce.root --hotpix 10

python scripts/cluster_analysis.py --dev c4379 --runfile ../data/c4379.root --hotpix 0.5
