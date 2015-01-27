#!/bin/sh

rm ../results/histos.root

python scripts/cluster_analysis.py --dev 08f89 --runfile ../data/08f89.root --hotpix 1

python scripts/cluster_analysis.py --dev 23587 --runfile ../data/23587.root --hotpix 1

python scripts/cluster_analysis.py --dev 6b24c --runfile ../data/6b24c.root --hotpix 1

python scripts/cluster_analysis.py --dev 6d7bc --runfile ../data/6d7bc.root --hotpix 1

python scripts/cluster_analysis.py --dev 95ea0 --runfile ../data/95ea0.root --hotpix 1

python scripts/cluster_analysis.py --dev 960d6 --runfile ../data/960d6.root --hotpix 100
