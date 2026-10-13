import os
import matplotlib.pyplot as plt
import numpy as np


def ensure_plots_dir(output_dir):
    os.makedirs(output_dir, exist_ok=True)


def plot_comparison(dates, y_true, arima_pred, lstm_pred, output_dir):
    ensure_plots_dir(output_dir)
    x = np.arange(len(dates))
    plt.figure(figsize=(8,4))
    plt.plot(x, y_true, label='True', marker='o')
    plt.plot(x, arima_pred, label='ARIMA', marker='x')
    plt.plot(x, lstm_pred, label='LSTM', marker='s')
    plt.legend()
    plt.title('7-day Prediction Comparison')
    plt.xlabel('Day')
    plt.xticks(x, dates, rotation=45)
    plt.tight_layout()
    path = os.path.join(output_dir, 'prediction_comparison.png')
    plt.savefig(path)
    plt.close()

    err_ar = np.abs(np.asarray(y_true) - np.asarray(arima_pred))
    err_ls = np.abs(np.asarray(y_true) - np.asarray(lstm_pred))
    plt.figure(figsize=(6,3))
    plt.hist([err_ar, err_ls], bins=10, label=['ARIMA','LSTM'], alpha=0.7)
    plt.legend()
    plt.title('Absolute Error Distribution')
    plt.tight_layout()
    path2 = os.path.join(output_dir, 'error_distribution.png')
    plt.savefig(path2)
    plt.close()
