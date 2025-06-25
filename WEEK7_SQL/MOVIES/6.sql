SELECT AVG(rating) AS ratings_average
 FROM ratings
WHERE movie_id IN
    (SELECT id FROM movies WHERE year = 2012);
    