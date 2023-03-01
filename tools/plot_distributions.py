import pandas as pd
from matplotlib import pyplot as plt
import seaborn as sns


def main():
    data = pd.read_csv("result.txt")
    slice = data[data["Name"] == "SINGLE_ARG(Storage<uint64_t, ArrayMap<uint64_t, 10000>>)"]
    fig, ax = plt.subplots(3, 1)
    sns.lineplot(data=slice, x='Dataset Size',
                 y="Insertion (ns)", hue="Distribution Type", ax=ax[0])
    sns.lineplot(data=slice, x='Dataset Size',
                 y="Query (ns)", hue="Distribution Type", ax=ax[1])
    sns.lineplot(data=slice, x='Dataset Size',
                 y="External Invokes", hue="Distribution Type", ax=ax[2])
    for a in ax:
        a.set_xscale('log')
    plt.show()


if __name__ == '__main__':
    main()
