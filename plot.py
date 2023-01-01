from sys import stdin
import pandas as pd
from matplotlib import pyplot as plt
import seaborn as sns
import argparse

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--output', default='test-storage.pdf')
    args = parser.parse_args()

    data = pd.read_csv(stdin)
    data['Name'] = data['Name'].map(lambda x:
        x.removeprefix('SINGLE_ARG(Storage<uint64_t, ')
         .removesuffix('>)'))
    print(data)
    fig, ax = plt.subplots(3, 1, figsize=(9, 15))
    sns.lineplot(data=data, x='Dataset Size',
                 y="Insertion (ns)", hue="Name", ax=ax[0])
    sns.lineplot(data=data, x='Dataset Size',
                 y="Query (ns)", hue="Name", ax=ax[1])
    sns.lineplot(data=data, x='Dataset Size',
                 y="Average External Accesses", hue="Name", ax=ax[2])
    for a in ax:
        a.set_xscale('log')
    plt.savefig(args.output)


if __name__ == '__main__':
    main()
