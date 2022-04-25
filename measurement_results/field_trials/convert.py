#!/usr/bin/env python3

import glob
import pandas as pd


ddf = pd.DataFrame()
for f in glob.glob("*.ods"):
	ods = pd.read_excel(f, engine="odf", sheet_name=None)
	for sheet_name, df in ods.items():
		ddf = pd.concat([ddf, df])

ddf.to_csv("results.csv", index=False)
