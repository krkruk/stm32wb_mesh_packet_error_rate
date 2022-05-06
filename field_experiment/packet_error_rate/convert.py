#!/usr/bin/env python3

import glob
import pandas as pd


def rename_column_contents(label):
	if label == "forest":
		return "teren leśny"
	elif label == "city":
		return "teren zurbanizowany"
	
	return label

ddf = pd.DataFrame()
for f in glob.glob("*.ods"):
	ods = pd.read_excel(f, engine="odf", sheet_name=None)
	for sheet_name, df in ods.items():
		print(f"Reading sheet <{sheet_name}> in file: {f}")
		df["remote_received_packets"] = df["remote_received_packets"].apply(lambda value: max(0, value - 1))
		df["location"] = df["location"].apply(rename_column_contents)
		ddf = pd.concat([ddf, df])

ddf.to_csv("results.csv", index=False)
