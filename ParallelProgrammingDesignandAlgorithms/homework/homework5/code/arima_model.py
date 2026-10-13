import numpy as np
from statsmodels.tsa.arima.model import ARIMA


def arima_forecast(train_series, steps=7, order=(2,1,2)):
    """
    Fit ARIMA using statsmodels (fit allowed) and forecast using get_forecast.
    Returns a numpy array of length `steps` with predicted mean values.
    """
    model = ARIMA(train_series, order=tuple(order))
    res = model.fit()
    fc = res.get_forecast(steps=steps)
    pred = np.asarray(fc.predicted_mean)
    return pred
