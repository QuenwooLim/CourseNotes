import os
os.environ['HF_HOME'] = 'https://hf-mirror.com/'
import re
import numpy as np
from sklearn.feature_extraction.text import TfidfVectorizer
from typing import Tuple, List
from datasets import load_dataset

def load_imdb():
    dataset = load_dataset('imdb')

    for split, data in dataset.items():
        data.to_csv(f"imdb_{split}.csv", index=False)

    print("数据集已成功保存为CSV文件。")

try:
    from torchtext.datasets import IMDB
    _HAVE_TORCHTEXT = True
except Exception:
    _HAVE_TORCHTEXT = False

try:
    from datasets import load_dataset
    _HAVE_DATASETS = True
except Exception:
    _HAVE_DATASETS = False

import nltk
from nltk.corpus import stopwords


def ensure_nltk():
    try:
        stopwords.words('english')
    except Exception:
        nltk.download('punkt')
        nltk.download('stopwords')


def clean_text(text: str) -> str:
    text = text.lower()
    text = re.sub(r"<br\s*/?>", " ", text)
    text = re.sub(r"[^a-z0-9\s]", " ", text)
    text = re.sub(r"\s+", " ", text).strip()
    return text


def load_imdb_fallback(data_dir: str = None) -> Tuple[List[str], List[int], List[str], List[int]]:
    if data_dir is None:
        possible = [
            os.path.join(os.getcwd(), '..', 'aclImdb'),
            os.path.join(os.getcwd(), '..', '..', 'aclImdb'),
            os.path.join(os.getcwd(), 'aclImdb'),
        ]
    else:
        possible = [data_dir]

    for p in possible:
        p = os.path.abspath(p)
        if os.path.isdir(p):
            train_pos = os.path.join(p, 'train', 'pos')
            train_neg = os.path.join(p, 'train', 'neg')
            test_pos = os.path.join(p, 'test', 'pos')
            test_neg = os.path.join(p, 'test', 'neg')
            if all(os.path.isdir(x) for x in [train_pos, train_neg, test_pos, test_neg]):
                def read_dir(d):
                    texts = []
                    for fn in os.listdir(d):
                        fp = os.path.join(d, fn)
                        if os.path.isfile(fp):
                            with open(fp, 'r', encoding='utf8', errors='ignore') as f:
                                texts.append(f.read())
                    return texts

                pos_train = read_dir(train_pos)
                neg_train = read_dir(train_neg)
                pos_test = read_dir(test_pos)
                neg_test = read_dir(test_neg)
                X_train = pos_train + neg_train
                y_train = [1]*len(pos_train) + [0]*len(neg_train)
                X_test = pos_test + neg_test
                y_test = [1]*len(pos_test) + [0]*len(neg_test)
                return X_train, np.array(y_train, dtype=int), X_test, np.array(y_test, dtype=int)

    raise FileNotFoundError('Could not find IMDB dataset in expected locations. Please provide aclImdb dataset.')


def load_imdb() -> Tuple[List[str], np.ndarray, List[str], np.ndarray]:
    ensure_nltk()
    base_dir = os.path.dirname(__file__)
    train_csv = os.path.join(base_dir, 'imdb_train.csv')
    test_csv = os.path.join(base_dir, 'imdb_test.csv')
    if os.path.isfile(train_csv) and os.path.isfile(test_csv):
        import pandas as pd
        df_train = pd.read_csv(train_csv)
        df_test = pd.read_csv(test_csv)
        X_train = df_train['text'].astype(str).tolist()
        y_train = df_train['label'].astype(int).to_numpy()
        X_test = df_test['text'].astype(str).tolist()
        y_test = df_test['label'].astype(int).to_numpy()
        return X_train, y_train, X_test, y_test
    if _HAVE_DATASETS:
        try:
            ds = load_dataset('imdb')
            train = ds['train']
            test = ds['test']
            X_train = [t['text'] for t in train]
            y_train = [int(t['label']) for t in train]
            X_test = [t['text'] for t in test]
            y_test = [int(t['label']) for t in test]
            return X_train, np.array(y_train, dtype=int), X_test, np.array(y_test, dtype=int)
        except Exception:
            pass

    if _HAVE_TORCHTEXT:
        try:
            train_iter = IMDB(split='train')
            test_iter = IMDB(split='test')
            X_train, y_train = [], []
            for label, text in train_iter:
                X_train.append(text)
                y_train.append(1 if label == 'pos' else 0)
            X_test, y_test = [], []
            for label, text in test_iter:
                X_test.append(text)
                y_test.append(1 if label == 'pos' else 0)
            return X_train, np.array(y_train, dtype=int), X_test, np.array(y_test, dtype=int)
        except Exception:
            pass

    return load_imdb_fallback()


def build_tfidf(X_train: List[str], X_test: List[str], max_features: int = 5000, min_df: int = 2):
    vec = TfidfVectorizer(max_features=max_features, stop_words='english', ngram_range=(1,2), min_df=min_df)
    X_train_tfidf = vec.fit_transform([clean_text(t) for t in X_train])
    X_test_tfidf = vec.transform([clean_text(t) for t in X_test])
    vocab = vec.get_feature_names_out()
    return X_train_tfidf.toarray(), X_test_tfidf.toarray(), vocab


def prepare_and_save_tfidf(out_path: str = 'data/imdb_tfidf.npz'):
    X_train, y_train, X_test, y_test = load_imdb()
    Xtr, Xte, vocab = build_tfidf(X_train, X_test)
    os.makedirs(os.path.dirname(out_path), exist_ok=True)
    np.savez_compressed(out_path, X_train=Xtr, X_test=Xte, y_train=y_train, y_test=y_test, vocab=vocab)
    return Xtr, y_train, Xte, y_test, vocab


def load_tfidf_from_npz(path: str = 'data/imdb_tfidf.npz'):
    if not os.path.isfile(path):
        raise FileNotFoundError(f'Could not find TF-IDF npz at {path}')
    d = np.load(path, allow_pickle=True)
    X_train = d['X_train']
    X_test = d['X_test']
    y_train = d['y_train']
    y_test = d['y_test']
    vocab = d['vocab']
    return X_train, y_train, X_test, y_test, vocab
