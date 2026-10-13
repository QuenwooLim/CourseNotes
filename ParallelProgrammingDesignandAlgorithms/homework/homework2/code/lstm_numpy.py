import numpy as np


def sigmoid(x):
    return 1.0 / (1.0 + np.exp(-x))


def dsigmoid(y):
    return y * (1 - y)


def dtanh(y):
    return 1 - y ** 2


class LSTMModel:
    def __init__(self, input_size=1, hidden_size=32, output_dim=1):
        self.input_size = input_size
        self.hidden_size = hidden_size
        self.output_dim = output_dim
        self._init_weights()

    def _init_weights(self):
        h = self.hidden_size
        m = h + self.input_size
        scale = 0.05
        self.Wf = np.random.randn(h, m) * scale
        self.Wi = np.random.randn(h, m) * scale
        self.Wo = np.random.randn(h, m) * scale
        self.Wg = np.random.randn(h, m) * scale
        self.bf = np.zeros((h,))
        self.bi = np.zeros((h,))
        self.bo = np.zeros((h,))
        self.bg = np.zeros((h,))
        self.Wy = np.random.randn(self.output_dim, h) * scale
        self.by = np.zeros((self.output_dim,))
        self.params = {
            'Wf': self.Wf, 'Wi': self.Wi, 'Wo': self.Wo, 'Wg': self.Wg,
            'bf': self.bf, 'bi': self.bi, 'bo': self.bo, 'bg': self.bg,
            'Wy': self.Wy, 'by': self.by
        }
        self.opt_m = {k: np.zeros_like(v) for k, v in self.params.items()}
        self.opt_v = {k: np.zeros_like(v) for k, v in self.params.items()}
        self.opt_t = 0

    def forward(self, x_seq):
        seq_len = len(x_seq)
        h = self.hidden_size
        xs = [np.array([float(x)]) for x in x_seq]
        hs = [np.zeros(h)]
        cs = [np.zeros(h)]
        cache = []
        for t in range(seq_len):
            x_t = xs[t]
            concat = np.concatenate([hs[-1], x_t])
            f_t = sigmoid(self.Wf.dot(concat) + self.bf)
            i_t = sigmoid(self.Wi.dot(concat) + self.bi)
            o_t = sigmoid(self.Wo.dot(concat) + self.bo)
            g_t = np.tanh(self.Wg.dot(concat) + self.bg)
            c_t = f_t * cs[-1] + i_t * g_t
            h_t = o_t * np.tanh(c_t)
            hs.append(h_t)
            cs.append(c_t)
            cache.append((concat, f_t, i_t, o_t, g_t, cs[-2], c_t, h_t))
        y = self.Wy.dot(hs[-1]) + self.by
        if self.output_dim == 1:
            return float(y), cache, hs, cs
        else:
            return np.asarray(y).reshape(self.output_dim,), cache, hs, cs

    def backward(self, dy, cache, hs, cs):
        seq_len = len(cache)
        h = self.hidden_size
        dWf = np.zeros_like(self.Wf)
        dWi = np.zeros_like(self.Wi)
        dWo = np.zeros_like(self.Wo)
        dWg = np.zeros_like(self.Wg)
        dbf = np.zeros_like(self.bf)
        dbi = np.zeros_like(self.bi)
        dbo = np.zeros_like(self.bo)
        dbg = np.zeros_like(self.bg)
        dWy = np.zeros_like(self.Wy)
        dby = np.zeros_like(self.by)

        dy_arr = np.asarray(dy)
        if dy_arr.ndim == 0:
            dy_arr = dy_arr.reshape(1,)
        dWy += np.outer(dy_arr, hs[-1])
        dby += dy_arr
        dh_next = self.Wy.T.dot(dy_arr)

        dc_next = np.zeros((h,))
        for t in reversed(range(seq_len)):
            concat, f_t, i_t, o_t, g_t, c_prev, c_t, h_t = cache[t]
            dh = dh_next.flatten()
            do = dh * np.tanh(c_t)
            do_raw = do * dsigmoid(o_t)

            dc = dh * o_t * dtanh(np.tanh(c_t)) + dc_next
            dg = dc * i_t
            dg_raw = dg * dtanh(g_t)
            di = dc * g_t
            di_raw = di * dsigmoid(i_t)
            df = dc * c_prev
            df_raw = df * dsigmoid(f_t)

            dWf += np.outer(df_raw, concat)
            dWi += np.outer(di_raw, concat)
            dWo += np.outer(do_raw, concat)
            dWg += np.outer(dg_raw, concat)
            dbf += df_raw
            dbi += di_raw
            dbo += do_raw
            dbg += dg_raw

            dconcat = (self.Wf.T.dot(df_raw) + self.Wi.T.dot(di_raw) +
                       self.Wo.T.dot(do_raw) + self.Wg.T.dot(dg_raw))

            dh_next = dconcat[:h].reshape(h,)
            dc_next = dc * f_t

        grads = {
            'Wf': dWf, 'Wi': dWi, 'Wo': dWo, 'Wg': dWg,
            'bf': dbf, 'bi': dbi, 'bo': dbo, 'bg': dbg,
            'Wy': dWy, 'by': dby
        }
        return grads

    def apply_grads(self, grads, lr=1e-3, clip=None):
        if clip is not None:
            total = 0.0
            for g in grads.values():
                total += np.sum(g.astype(float) ** 2)
            total_norm = np.sqrt(total)
            if total_norm > clip and total_norm > 0:
                factor = clip / (total_norm + 1e-12)
                for k in grads:
                    grads[k] = grads[k] * factor

        self.Wf -= lr * grads['Wf']
        self.Wi -= lr * grads['Wi']
        self.Wo -= lr * grads['Wo']
        self.Wg -= lr * grads['Wg']
        self.bf -= lr * grads['bf']
        self.bi -= lr * grads['bi']
        self.bo -= lr * grads['bo']
        self.bg -= lr * grads['bg']
        self.Wy -= lr * grads['Wy']
        self.by -= lr * grads['by']

    def adam_step(self, grads, lr=1e-3, beta1=0.9, beta2=0.999, eps=1e-8, clip=None):
        if clip is not None:
            total = 0.0
            for g in grads.values():
                total += np.sum(g.astype(float) ** 2)
            total_norm = np.sqrt(total)
            if total_norm > clip and total_norm > 0:
                factor = clip / (total_norm + 1e-12)
                for k in grads:
                    grads[k] = grads[k] * factor

        self.opt_t += 1
        for k, param in self.params.items():
            g = grads[k]
            self.opt_m[k] = beta1 * self.opt_m[k] + (1 - beta1) * g
            self.opt_v[k] = beta2 * self.opt_v[k] + (1 - beta2) * (g * g)
            m_hat = self.opt_m[k] / (1 - beta1 ** self.opt_t)
            v_hat = self.opt_v[k] / (1 - beta2 ** self.opt_t)
            update = lr * m_hat / (np.sqrt(v_hat) + eps)
            param -= update


def train_lstm_recursive(series, seq_len=30, hidden_size=32, epochs=20, lr=1e-3, clip=None, batch_size=32, optim='adam', horizon=1):
    X, y = [], []
    if horizon <= 1:
        for i in range(len(series) - seq_len):
            X.append(series[i:i+seq_len])
            y.append(series[i+seq_len])
        X = np.array(X, dtype=float)
        y = np.array(y, dtype=float)
    else:
        for i in range(len(series) - seq_len - horizon + 1):
            X.append(series[i:i+seq_len])
            y.append(series[i+seq_len:i+seq_len+horizon])
        X = np.array(X, dtype=float)
        y = np.array(y, dtype=float)

    model = LSTMModel(input_size=1, hidden_size=hidden_size, output_dim=horizon)
    n_samples = len(X)
    if batch_size is None or batch_size <= 0:
        batch_size = n_samples

    for ep in range(epochs):
        epoch_loss = 0.0
        idx = np.arange(n_samples)
        np.random.shuffle(idx)
        for start in range(0, n_samples, batch_size):
            batch_idx = idx[start:start+batch_size]
            grads_sum = None
            batch_loss = 0.0
            for i in batch_idx:
                x_seq = X[i]
                target = y[i]
                pred, cache, hs, cs = model.forward(x_seq)
                pred_arr = np.asarray(pred)
                target_arr = np.asarray(target)
                diff = pred_arr - target_arr
                loss = 0.5 * np.mean(diff ** 2)
                batch_loss += loss
                dy = diff
                grads = model.backward(dy, cache, hs, cs)
                if grads_sum is None:
                    grads_sum = {k: np.array(v, dtype=float) for k, v in grads.items()}
                else:
                    for k in grads_sum:
                        grads_sum[k] += grads[k]
            for k in grads_sum:
                grads_sum[k] /= len(batch_idx)
            if optim == 'adam':
                model.adam_step(grads_sum, lr=lr, clip=clip)
            else:
                model.apply_grads(grads_sum, lr=lr, clip=clip)
            epoch_loss += batch_loss
        if (ep+1) % 10 == 0 or ep == 0:
            avg_loss = epoch_loss / max(1, n_samples)
            print(f"Epoch {ep+1}/{epochs}, loss={avg_loss:.6f}")
    return model


def predict_recursive(model, series, seq_len=30, steps=7):
    if hasattr(model, 'output_dim') and model.output_dim > 1:
        seq = series[-seq_len:]
        pred_vec, _, _, _ = model.forward(seq)
        return np.array(pred_vec)[:steps]

    seq = list(series[-seq_len:])
    preds = []
    for _ in range(steps):
        pred, _, _, _ = model.forward(seq[-seq_len:])
        preds.append(pred)
        seq.append(pred)
    return np.array(preds)
