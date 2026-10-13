import os
import argparse
import numpy as np

from utils import load_stock_csv, make_windows, save_data_npz
from arima_model import arima_forecast
from lstm_numpy import train_lstm_recursive, predict_recursive
from metrics import mae, rmse
from plots import plot_comparison, ensure_plots_dir


def run(args):
    os.makedirs(args.output_dir, exist_ok=True)
    ensure_plots_dir(args.output_dir)

    dates, close = load_stock_csv(args.csv)
    n = len(close)
    test_len = 7
    train_close = close[:-test_len]
    test_close = close[-test_len:]
    dates_train = dates[:-test_len]
    dates_test = dates[-test_len:]

    if len(train_close) < 2:
        raise ValueError("Not enough data for returns-based LSTM training")
    train_returns = np.log(train_close[1:] / train_close[:-1])
    ret_mean = float(np.mean(train_returns))
    ret_std = float(np.std(train_returns)) if float(np.std(train_returns)) > 0 else 1.0
    train_returns_norm = (train_returns - ret_mean) / ret_std

    npz_path = os.path.join(args.output_dir, "stock_data.npz")
    save_data_npz(npz_path, train_close=train_close, test_close=test_close,
                  dates_train=dates_train, dates_test=dates_test,
                  ret_mean=ret_mean, ret_std=ret_std)

    arima_pred = arima_forecast(train_close, steps=test_len, order=args.arima_order)

    seq_len = args.seq_len
    horizon = args.horizon
    lstm_model = train_lstm_recursive(train_returns_norm, seq_len=seq_len,
                                      hidden_size=args.hidden_size,
                                      epochs=args.epochs, lr=args.lr, clip=args.clip,
                                      batch_size=args.batch_size, optim='adam', horizon=horizon)

    if len(train_returns_norm) < seq_len:
        raise ValueError("seq_len too large for available training returns")
    seq_norm = list(train_returns_norm[-seq_len:])
    pred_norm_vec, _, _, _ = lstm_model.forward(seq_norm)
    pred_norm_vec = np.asarray(pred_norm_vec).reshape(-1)[:test_len]
    last_price = float(train_close[-1])
    lstm_price_preds = []
    for p_norm in pred_norm_vec:
        p_ret = float(p_norm) * ret_std + ret_mean
        last_price = last_price * np.exp(p_ret)
        lstm_price_preds.append(last_price)
    lstm_pred = np.array(lstm_price_preds)

    arima_mae = mae(test_close, arima_pred)
    arima_rmse = rmse(test_close, arima_pred)
    lstm_mae = mae(test_close, lstm_pred)
    lstm_rmse = rmse(test_close, lstm_pred)

    print("ARIMA MAE/RMSE:", arima_mae, arima_rmse)
    print("LSTM MAE/RMSE:", lstm_mae, lstm_rmse)

    plot_comparison(dates_test, test_close, arima_pred, lstm_pred, args.output_dir)


def parse_args():
    p = argparse.ArgumentParser()
    p.add_argument("--csv", default="stock.csv", help="input CSV path with Date,Close")
    p.add_argument("--output-dir", default="../plots", help="plots and data output dir")
    p.add_argument("--arima-order", nargs=3, type=int, default=[2,1,2], help="p d q")
    p.add_argument("--seq-len", dest="seq_len", type=int, default=30)
    p.add_argument("--epochs", type=int, default=50)
    p.add_argument("--lr", type=float, default=0.001)
    p.add_argument("--hidden-size", dest="hidden_size", type=int, default=32)
    p.add_argument("--clip", type=float, default=1.0, help="gradient clipping norm (None to disable)")
    p.add_argument("--batch-size", dest="batch_size", type=int, default=32)
    p.add_argument("--horizon", dest="horizon", type=int, default=7, help="prediction horizon")
    return p.parse_args()


if __name__ == '__main__':
    args = parse_args()
    run(args)
