SELECT title, rating
  FROM movies
  JOIN ratings ON movies.id = ratings.movie_id
 WHERE movies.year = 2010
   AND rating IS NOT NULL
ORDER BY ratings.rating DESC, title;
