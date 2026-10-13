import os
import numpy as np
import pandas as pd


def load_stock_csv(path):
    if not os.path.exists(path):
        raise FileNotFoundError(f"CSV file not found: {path}")
    df = pd.read_csv(path, parse_dates=["Date"])        
    df = df.dropna(subset=["Date"]).copy()
    df['Close'] = pd.to_numeric(df['Close'], errors='coerce')
    df = df.sort_values("Date").dropna(subset=["Close"]).reset_index(drop=True)
    dates = df["Date"].dt.strftime("%Y-%m-%d").tolist()
    close = df["Close"].values.astype(float)
    return dates, close


def make_windows(series, seq_len):
    X = []
    y = []
    for i in range(len(series) - seq_len):
        X.append(series[i:i+seq_len])
        y.append(series[i+seq_len])
    return np.array(X, dtype=float), np.array(y, dtype=float)


def save_data_npz(path, **kwargs):
    dirp = os.path.dirname(path)
    if dirp:
        os.makedirs(dirp, exist_ok=True)
    np.savez(path, **kwargs)
