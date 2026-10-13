import os
import numpy as np
import pandas as pd

def download_and_prepare(ticker='AAPL', start='2020-01-01', end='2023-12-31', out_csv='stock.csv'):
    try:
        import yfinance as yf
    except Exception as e:
        raise RuntimeError('yfinance is required. Install via pip install yfinance') from e

    df = yf.download(ticker, start=start, end=end)
    if df.empty:
        raise RuntimeError('No data downloaded for ticker {}'.format(ticker))

    df = df[['Close']].rename(columns={'Close': 'Close'})
    df = df.asfreq('B').ffill()
    df['returns'] = np.log(df['Close'] / df['Close'].shift(1))
    df['ma_7'] = df['Close'].rolling(7).mean()
    df['volatility'] = df['returns'].rolling(21).std()

    df = df.dropna().reset_index()
    out_path = os.path.join(os.path.dirname(__file__), out_csv)
    df.to_csv(out_path, index=False)
    print('Saved CSV to', out_path)


if __name__ == '__main__':
    download_and_prepare()
