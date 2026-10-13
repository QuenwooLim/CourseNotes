"""
Hybrid Movie Recommendation System
Combines collaborative filtering and content-based recommendation approaches
"""

import pandas as pd
import numpy as np
from sklearn.metrics.pairwise import cosine_similarity
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.decomposition import TruncatedSVD
import warnings
warnings.filterwarnings('ignore')


class HybridRecommender:
    """Hybrid recommendation system combining multiple recommendation strategies."""
    
    def __init__(self):
        """Initialize the hybrid recommender system."""
        # Data storage
        self.movies_df: pd.DataFrame = None
        self.ratings_df: pd.DataFrame = None
        
        # Collaborative filtering matrices
        self.user_item_matrix: pd.DataFrame = None
        self.user_similarity_matrix: pd.DataFrame = None
        self.item_similarity_matrix: pd.DataFrame = None
        
        # Content-based filtering components
        self.tfidf_matrix = None
        self.content_similarity_matrix = None
        
        # Dimensionality reduction
        self.svd_model = None
        
    def load_data(self, movies_path: str = 'ml-latest-small/movies.csv', 
                  ratings_path: str = 'ml-latest-small/ratings.csv') -> None:
        """
        Load MovieLens dataset from CSV files.
        
        Args:
            movies_path: Path to movies CSV file
            ratings_path: Path to ratings CSV file
        """
        print("\n[INFO] Loading dataset...")
        try:
            self.movies_df = pd.read_csv(movies_path)
            self.ratings_df = pd.read_csv(ratings_path)
            print(f"[SUCCESS] Dataset loaded: {len(self.movies_df):,} movies, {len(self.ratings_df):,} ratings")
        except FileNotFoundError as e:
            print(f"[ERROR] File not found: {e}")
            raise
        except Exception as e:
            print(f"[ERROR] Failed to load data: {e}")
            raise
    
    def preprocess_data(self) -> None:
        """
        Preprocess data and compute similarity matrices for recommendation algorithms.
        This includes user-item matrix creation, similarity computations, and dimensionality reduction.
        """
        print("\n[INFO] Starting data preprocessing...")
        
        # Create user-item interaction matrix
        # print("[STEP 1/5] Creating user-item matrix...")
        self.user_item_matrix = self.ratings_df.pivot_table(
            index='userId', 
            columns='movieId', 
            values='rating',
            fill_value=0
        )
        
        # Compute user-user similarity matrix (collaborative filtering)
        # print("[STEP 2/5] Computing user similarity matrix...")
        user_similarity_array = cosine_similarity(self.user_item_matrix)
        self.user_similarity_matrix = pd.DataFrame(
            user_similarity_array,
            index=self.user_item_matrix.index,
            columns=self.user_item_matrix.index
        )
        
        # Compute item-item similarity matrix (collaborative filtering)
        # print("[STEP 3/5] Computing item similarity matrix...")
        item_user_matrix = self.user_item_matrix.T
        item_similarity_array = cosine_similarity(item_user_matrix)
        self.item_similarity_matrix = pd.DataFrame(
            item_similarity_array,
            index=item_user_matrix.index,
            columns=item_user_matrix.index
        )
        
        # Compute content-based similarity using movie genres
        # print("[STEP 4/5] Computing content-based similarity...")
        self.movies_df['genres'] = self.movies_df['genres'].fillna('')
        tfidf_vectorizer = TfidfVectorizer(stop_words='english')
        self.tfidf_matrix = tfidf_vectorizer.fit_transform(self.movies_df['genres'])
        self.content_similarity_matrix = cosine_similarity(self.tfidf_matrix)
        
        # Apply SVD for dimensionality reduction (memory optimization)
        # print("[STEP 5/5] Applying SVD dimensionality reduction...")
        self.svd_model = TruncatedSVD(n_components=50, random_state=42)
        _ = self.svd_model.fit_transform(self.user_item_matrix)
        
        print("[SUCCESS] Data preprocessing completed!\n")
    
    def user_based_collaborative_filtering(self, user_id: int, n_recommendations: int = 10) -> list:
        """
        Generate recommendations using user-based collaborative filtering.
        
        Args:
            user_id: Target user ID
            n_recommendations: Number of recommendations to generate
            
        Returns:
            List of recommended movie IDs
        """
        # Validate user existence
        if user_id not in self.user_similarity_matrix.index:
            print(f"[WARNING] User {user_id} not found in similarity matrix")
            return []
        
        # Find top similar users (excluding the target user)
        similar_users = self.user_similarity_matrix.loc[user_id].sort_values(
            ascending=False
        ).iloc[1:11]
        
        # Aggregate recommendations from similar users
        recommendation_scores = {}
        for similar_user_id, similarity_score in similar_users.items():
            # Get movies rated by similar user
            similar_user_ratings = self.user_item_matrix.loc[similar_user_id]
            rated_movies = similar_user_ratings[similar_user_ratings > 0].index
            
            # Score unrated movies for target user
            for movie_id in rated_movies:
                if self.user_item_matrix.loc[user_id, movie_id] == 0:  # User hasn't rated this movie
                    if movie_id not in recommendation_scores:
                        recommendation_scores[movie_id] = 0
                    recommendation_scores[movie_id] += similarity_score * similar_user_ratings[movie_id]
        
        # Sort and return top recommendations
        sorted_recommendations = sorted(
            recommendation_scores.items(), 
            key=lambda x: x[1], 
            reverse=True
        )
        return [movie_id for movie_id, _ in sorted_recommendations[:n_recommendations]]
    
    def item_based_collaborative_filtering(self, user_id: int, n_recommendations: int = 10) -> list:
        """
        Generate recommendations using item-based collaborative filtering.
        
        Args:
            user_id: Target user ID
            n_recommendations: Number of recommendations to generate
            
        Returns:
            List of recommended movie IDs
        """
        # Validate user existence
        if user_id not in self.user_item_matrix.index:
            print(f"[WARNING] User {user_id} not found in user-item matrix")
            return []
        
        # Get user's rated movies
        user_ratings = self.user_item_matrix.loc[user_id]
        rated_movies = user_ratings[user_ratings > 0].index
        
        # Aggregate recommendations based on similar items
        recommendation_scores = {}
        for rated_movie_id in rated_movies:
            if rated_movie_id not in self.item_similarity_matrix.index:
                continue
            
            # Find similar movies to each rated movie
            similar_movies = self.item_similarity_matrix.loc[rated_movie_id].sort_values(
                ascending=False
            ).iloc[1:11]
            
            # Score unrated similar movies
            for movie_id, similarity_score in similar_movies.items():
                if user_ratings.get(movie_id, 0) == 0:  # User hasn't rated this movie
                    if movie_id not in recommendation_scores:
                        recommendation_scores[movie_id] = 0
                    recommendation_scores[movie_id] += similarity_score * user_ratings[rated_movie_id]
        
        # Sort and return top recommendations
        sorted_recommendations = sorted(
            recommendation_scores.items(), 
            key=lambda x: x[1], 
            reverse=True
        )
        return [movie_id for movie_id, _ in sorted_recommendations[:n_recommendations]]
    
    def content_based_recommendation(self, movie_id: int, n_recommendations: int = 10) -> list:
        """
        Generate recommendations based on content similarity (movie genres).
        
        Args:
            movie_id: Reference movie ID
            n_recommendations: Number of recommendations to generate
            
        Returns:
            List of similar movie IDs
        """
        # Validate movie existence
        if movie_id not in self.movies_df['movieId'].values:
            print(f"[WARNING] Movie {movie_id} not found in dataset")
            return []
        
        # Find movie index and get similarity scores
        movie_idx = self.movies_df[self.movies_df['movieId'] == movie_id].index[0]
        similarity_scores = list(enumerate(self.content_similarity_matrix[movie_idx]))
        
        # Sort by similarity and exclude the reference movie
        similarity_scores = sorted(similarity_scores, key=lambda x: x[1], reverse=True)[1:n_recommendations+1]
        
        # Return similar movie IDs
        return [self.movies_df.iloc[idx]['movieId'] for idx, _ in similarity_scores]
    
    def hybrid_recommendation(self, user_id: int, n_recommendations: int = 10, 
                             cf_weight: float = 0.6, content_weight: float = 0.4) -> list:
        """
        Generate hybrid recommendations combining multiple approaches.
        
        Args:
            user_id: Target user ID
            n_recommendations: Number of recommendations to generate
            cf_weight: Weight for collaborative filtering component
            content_weight: Weight for content-based component
            
        Returns:
            List of recommended movie IDs
        """
        print(f"\n[INFO] Generating hybrid recommendations for user {user_id}...")
        
        # Get user's rating history
        user_history = self.ratings_df[self.ratings_df['userId'] == user_id]
        
        # Handle cold-start scenario (users with limited history)
        if len(user_history) < 5:
            cf_weight = 0.4
            content_weight = 0.3
            popularity_weight = 0.3
            print("[NOTICE] Cold-start user detected. Adjusting recommendation weights...")
        else:
            popularity_weight = 0.0
        
        # Step 1: Collaborative filtering recommendations
        print("[HYBRID] Computing collaborative filtering recommendations...")
        cf_recommendations = {}
        
        # User-based CF
        user_cf_recs = self.user_based_collaborative_filtering(user_id, n_recommendations * 2)
        for movie_id in user_cf_recs:
            cf_recommendations[movie_id] = cf_recommendations.get(movie_id, 0) + 1.0
        
        # Item-based CF
        item_cf_recs = self.item_based_collaborative_filtering(user_id, n_recommendations * 2)
        for movie_id in item_cf_recs:
            cf_recommendations[movie_id] = cf_recommendations.get(movie_id, 0) + 1.0
        
        # Step 2: Content-based recommendations
        print("[HYBRID] Computing content-based recommendations...")
        content_recommendations = {}
        if len(user_history) > 0:
            # Use top 3 recently watched movies for content-based recommendations
            recent_movies = user_history.nlargest(3, 'timestamp')['movieId'].values
            for movie_id in recent_movies:
                content_recs = self.content_based_recommendation(movie_id, n_recommendations)
                for rec_movie_id in content_recs:
                    content_recommendations[rec_movie_id] = content_recommendations.get(rec_movie_id, 0) + 1.0
        
        # Step 3: Popularity-based recommendations (for cold-start)
        print("[HYBRID] Computing popularity-based recommendations...")
        popularity_recommendations = {}
        if popularity_weight > 0:
            movie_popularity = self.ratings_df.groupby('movieId').size()
            popular_movies = movie_popularity.nlargest(n_recommendations * 2).index
            for movie_id in popular_movies:
                popularity_recommendations[movie_id] = 1.0
        
        # Step 4: Combine scores with weighted fusion
        print("[HYBRID] Combining recommendation scores...")
        combined_scores = {}
        
        # Add collaborative filtering scores
        for movie_id, score in cf_recommendations.items():
            combined_scores[movie_id] = combined_scores.get(movie_id, 0) + score * cf_weight
        
        # Add content-based scores
        for movie_id, score in content_recommendations.items():
            combined_scores[movie_id] = combined_scores.get(movie_id, 0) + score * content_weight
        
        # Add popularity scores
        for movie_id, score in popularity_recommendations.items():
            combined_scores[movie_id] = combined_scores.get(movie_id, 0) + score * popularity_weight
        
        # Step 5: Apply diversity penalty to avoid genre over-concentration
        print("[HYBRID] Applying diversity optimization...")
        final_recommendations = []
        genre_frequency = {}
        
        for movie_id, score in sorted(combined_scores.items(), key=lambda x: x[1], reverse=True):
            if len(final_recommendations) >= n_recommendations:
                break
            
            # Get movie information
            movie_info = self.movies_df[self.movies_df['movieId'] == movie_id]
            if len(movie_info) == 0:
                continue
            
            # Extract genres
            genres = movie_info.iloc[0]['genres'].split('|')
            
            # Calculate diversity penalty based on genre frequency
            diversity_penalty = 0
            for genre in genres:
                if genre in genre_frequency:
                    diversity_penalty += genre_frequency[genre] * 0.1
            
            # Apply penalty and filter
            final_score = score - diversity_penalty
            if final_score > 0:
                final_recommendations.append(movie_id)
                for genre in genres:
                    genre_frequency[genre] = genre_frequency.get(genre, 0) + 1
        
        print(f"[SUCCESS] Generated {len(final_recommendations)} hybrid recommendations")
        return final_recommendations
    
    def get_movie_details(self, movie_ids: list) -> list:
        """
        Retrieve detailed information for a list of movie IDs.
        
        Args:
            movie_ids: List of movie IDs
            
        Returns:
            List of dictionaries containing movie details
        """
        movie_details = []
        for movie_id in movie_ids:
            movie_data = self.movies_df[self.movies_df['movieId'] == movie_id]
            if len(movie_data) > 0:
                movie_row = movie_data.iloc[0]
                movie_details.append({
                    'movie_id': int(movie_row['movieId']),
                    'title': str(movie_row['title']),
                    'genres': str(movie_row['genres'])
                })
        return movie_details
    
    def evaluate_recommendations(self, user_id: int, test_ratio: float = 0.2) -> dict:
        """
        Evaluate recommendation quality using hold-out validation.
        
        Args:
            user_id: Target user ID
            test_ratio: Proportion of recent ratings to use as test set
            
        Returns:
            Dictionary containing evaluation metrics
        """
        # Get user's rating history
        user_ratings = self.ratings_df[self.ratings_df['userId'] == user_id]
        
        # Validate sufficient data
        if len(user_ratings) < 10:
            return {"error": "Insufficient user rating data for evaluation"}
        
        # Split into training and test sets
        test_size = int(len(user_ratings) * test_ratio)
        if test_size <= 0:
            return {"error": "Test set size is zero"}
        
        # Use most recent ratings as test set
        test_movies = user_ratings.nlargest(test_size, 'timestamp')['movieId'].values
        
        # Temporarily modify dataset for evaluation
        original_ratings = self.ratings_df.copy()
        
        try:
            # Create training set by removing test ratings
            train_ratings = self.ratings_df[~(
                (self.ratings_df['userId'] == user_id) & 
                (self.ratings_df['movieId'].isin(test_movies))
            )]
            
            # Rebuild models with training data only
            self.ratings_df = train_ratings
            self.preprocess_data()
            
            # Generate recommendations
            recommendations = self.hybrid_recommendation(user_id, n_recommendations=20)
            
        finally:
            # Restore original dataset
            self.ratings_df = original_ratings
            self.preprocess_data()
        
        # Calculate hit rate
        hits = len(set(recommendations) & set(test_movies))
        hit_rate = hits / len(test_movies) if len(test_movies) > 0 else 0
        
        return {
            "user_id": user_id,
            "test_set_size": len(test_movies),
            "recommendation_count": len(recommendations),
            "hits": hits,
            "hit_rate": hit_rate,
            "precision": hits / len(recommendations) if len(recommendations) > 0 else 0
        }
    
    def evaluate_single_algorithm(self, user_id: int, algorithm: str, 
                                  test_ratio: float = 0.2, n_recommendations: int = 10) -> dict:
        """
        Evaluate performance of a single recommendation algorithm.
        
        Args:
            user_id: Target user ID
            algorithm: Algorithm name ('user_cf', 'item_cf', or 'content')
            test_ratio: Proportion of recent ratings for testing
            n_recommendations: Number of recommendations to generate
            
        Returns:
            Dictionary containing evaluation metrics
        """
        # Get user's rating history
        user_ratings = self.ratings_df[self.ratings_df['userId'] == user_id]
        
        # Validate sufficient data
        if len(user_ratings) < 10:
            return None
        
        # Prepare test set
        test_size = int(len(user_ratings) * test_ratio)
        if test_size <= 0:
            return None
        
        test_movies = user_ratings.nlargest(test_size, 'timestamp')['movieId'].values
        
        # Temporarily modify dataset
        original_ratings = self.ratings_df.copy()
        
        try:
            # Create training set
            train_ratings = self.ratings_df[~(
                (self.ratings_df['userId'] == user_id) & 
                (self.ratings_df['movieId'].isin(test_movies))
            )]
            self.ratings_df = train_ratings
            self.preprocess_data()
            
            # Generate recommendations based on algorithm
            recommendations = []
            if algorithm == 'user_cf':
                recommendations = self.user_based_collaborative_filtering(user_id, n_recommendations)
            elif algorithm == 'item_cf':
                recommendations = self.item_based_collaborative_filtering(user_id, n_recommendations)
            elif algorithm == 'content':
                recent_movies = user_ratings.nlargest(3, 'timestamp')['movieId'].values
                if len(recent_movies) > 0:
                    recommendations = self.content_based_recommendation(recent_movies[0], n_recommendations)
            
        finally:
            # Restore original dataset
            self.ratings_df = original_ratings
            self.preprocess_data()
        
        # Calculate recall
        hits = len(set(recommendations) & set(test_movies))
        recall = hits / len(test_movies) if len(test_movies) > 0 else 0
        
        return {
            "algorithm": algorithm,
            "recall": recall,
            "hits": hits,
            "test_set_size": len(test_movies),
            "recommendation_count": len(recommendations)
        }


def display_recommendations(recommender: HybridRecommender, user_id: int, 
                          recommendations: list) -> None:
    """
    Display formatted recommendation results for a user.
    
    Args:
        recommender: HybridRecommender instance
        user_id: User ID
        recommendations: List of recommended movie IDs
    """
    if not recommendations:
        print(f"  [INFO] No recommendations generated for user {user_id}")
        return
    
    print(f"\n  Recommended Movies for User {user_id}:")
    print("  " + "-" * 60)
    
    movie_details = recommender.get_movie_details(recommendations)
    for idx, movie in enumerate(movie_details, 1):
        print(f"  {idx:2d}. {movie['title'][:50]:<50} | {movie['genres']}")
    
    print("  " + "-" * 60)


def display_user_history(recommender: HybridRecommender, user_id: int) -> None:
    """
    Display user's recent viewing history.
    
    Args:
        recommender: HybridRecommender instance
        user_id: User ID
    """
    user_history = recommender.ratings_df[recommender.ratings_df['userId'] == user_id]
    
    if len(user_history) == 0:
        print(f"  [INFO] No viewing history found for user {user_id}")
        return
    
    print(f"\n  User {user_id} Viewing History:")
    print(f"  Total ratings: {len(user_history):,}")
    
    if len(user_history) > 0:
        print(f"\n  Recently Watched:")
        recent_movies = user_history.nlargest(3, 'timestamp')
        for _, rating_row in recent_movies.iterrows():
            movie_info = recommender.movies_df[
                recommender.movies_df['movieId'] == rating_row['movieId']
            ]
            if len(movie_info) > 0:
                title = movie_info.iloc[0]['title']
                print(f"    • {title[:45]:<45} (Rating: {rating_row['rating']:.1f})")


def display_evaluation_results(evaluation: dict, algorithm_name: str = "Hybrid") -> None:
    """
    Display formatted evaluation results.
    
    Args:
        evaluation: Evaluation metrics dictionary
        algorithm_name: Name of the evaluated algorithm
    """
    print(f"\n  {algorithm_name} Algorithm Evaluation:")
    print("  " + "-" * 40)
    print(f"    Test Set Size:     {evaluation.get('test_set_size', 0):>6}")
    print(f"    Hits:              {evaluation.get('hits', 0):>6}")
    print(f"    Hit Rate:          {evaluation.get('hit_rate', 0):>6.2%}")
    if 'recall' in evaluation:
        print(f"    Recall:            {evaluation['recall']:>6.2%}")
    if 'precision' in evaluation:
        print(f"    Precision:         {evaluation['precision']:>6.2%}")


def main():
    """Main execution function for the hybrid recommendation system."""
    
    print("\n" + "=" * 70)
    print(" " * 15 + "HYBRID MOVIE RECOMMENDATION SYSTEM")
    print("=" * 70)
    
    # Initialize recommender system
    print("\n[SYSTEM] Initializing recommendation engine...")
    recommender = HybridRecommender()
    
    # Load and preprocess data
    try:
        recommender.load_data()
        recommender.preprocess_data()
    except Exception as e:
        print(f"[ERROR] System initialization failed: {e}")
        return
    
    # Demonstration with sample users
    sample_users = [4, 16, 27, 69, 99, 101, 110, 120, 134, 180]
    print(f"[SYSTEM] Starting recommendation demonstration for {len(sample_users)} users...\n")
    
    for user_id in sample_users:
        print(f"\n{'=' * 70}")
        print(f"USER {user_id:4d} RECOMMENDATION SESSION")
        print(f"{'=' * 70}")
        
        # Display user history
        display_user_history(recommender, user_id)
        
        # Generate and display hybrid recommendations
        print(f"\n[RECOMMENDER] Generating hybrid recommendations...")
        recommendations = recommender.hybrid_recommendation(user_id, n_recommendations=10)
        display_recommendations(recommender, user_id, recommendations)
        
        # Evaluate recommendation quality for users with sufficient data
        user_history = recommender.ratings_df[recommender.ratings_df['userId'] == user_id]
        if len(user_history) >= 10:
            print(f"\n[RECOMMENDER] Evaluating recommendation quality...")
            
            # Evaluate hybrid approach
            hybrid_evaluation = recommender.evaluate_recommendations(user_id)
            if 'error' not in hybrid_evaluation:
                display_evaluation_results(hybrid_evaluation, "Hybrid")
            
            # Compare individual algorithms
            print(f"\n  Individual Algorithm Comparison:")
            print("  " + "-" * 40)
            
            algorithms = ['user_cf', 'item_cf', 'content']
            algorithm_names = {
                'user_cf': 'User-Based CF',
                'item_cf': 'Item-Based CF', 
                'content': 'Content-Based'
            }
            
            for algo in algorithms:
                eval_result = recommender.evaluate_single_algorithm(user_id, algo)
                if eval_result:
                    recall_pct = eval_result['recall'] * 100
                    print(f"    {algorithm_names[algo]:<18} Recall: {recall_pct:>5.1f}%")
    
    print("\n" + "=" * 70)
    print(" " * 20 + "RECOMMENDATION DEMONSTRATION COMPLETED")
    print("=" * 70)
    print("\n[SYSTEM] Thank you for using the Hybrid Movie Recommendation System!\n")


if __name__ == "__main__":
    main()