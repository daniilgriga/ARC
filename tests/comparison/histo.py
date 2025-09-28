import pandas as pd
import matplotlib.pyplot as plt

def main ():
    df = pd.read_csv ("comparison_results.csv")

    plt.figure (figsize=(14, 7))

    plt.bar (
        [x - 0.2 for x in range(len(df))],
        df['arc_hits'],
        width=0.4,
        label='ARC Hits',
        color='blue'
    )

    plt.bar (
        [x + 0.2 for x in range(len(df))],
        df['ideal_hits'],
        width=0.4,
        label='Ideal Hits',
        color='lightcoral'
    )

    plt.xlabel ('Test ID')
    plt.ylabel ('Number of Hits')
    plt.title ('ARC vs Ideal Cache: Hit Comparison on Stress Tests')
    plt.xticks (range(len(df)), df['test_id'], rotation=45)
    plt.legend()
    plt.tight_layout()

    plt.savefig ("cache_comparison.png", dpi=150)
    print ("Histogram saved as cache_comparison.png")
    plt.show()

if __name__ == "__main__":
    main()
