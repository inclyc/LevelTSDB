import pandas as pd
from matplotlib import pyplot as plt
import seaborn as sns


def main():
    data = pd.read_csv("result.txt")
    fig, ax = plt.subplots(3, 1)
    sns.lineplot(data=data, x='Dataset Size',
                 y="Insertion (ns)", hue="Name", ax=ax[0])
    sns.lineplot(data=data, x='Dataset Size',
                 y="Query (ns)", hue="Name", ax=ax[1])
    sns.lineplot(data=data, x='Dataset Size',
                 y="External Invokes", hue="Name", ax=ax[2])
    for a in ax:
        a.set_xscale('log')
    plt.show()


if __name__ == '__main__':
    main()
