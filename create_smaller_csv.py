import pandas as pd

sizes = [100, 1000, 10000]

df = pd.read_csv("dataset/100000.csv")

for s in sizes:
    out_name = f"{s}.csv"
    df.head(s).to_csv(out_name, index=False)