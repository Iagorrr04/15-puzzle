import pandas as pd
import os

final = pd.DataFrame()

df = pd.DataFrame()
for csv in os.listdir("benchmark/csvs"):
    df = pd.read_csv("benchmark/csvs/"+csv)
    print(df)
    final = final.append(df, ignore_index=True)

# print(final.columns.tolist())
# input()

a = open("teste.html", "w")
a.write(final.to_html())
a.close()
