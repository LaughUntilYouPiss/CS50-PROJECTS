-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Find crime scene description
SELECT description
FROM crime_scene_reports
WHERE month = 7 AND day = 28
AND street = 'Humphrey Street';
/*  Theft took place at 10:15am at the Humphrey Street bakery.
    Interviews were conducted today with three witnesses – each mentions the bakery.
    Littering took place at 16:36. No known witnesses. */

-- Find the witness interviews
SELECT transcript
FROM interviews
WHERE month = 7 AND day = 28;
/*  • Sometime within ten minutes of the theft, the thief get into a car in the bakery parking lot and drove away
    --> SECURITY FOOTAGE
    • The thief withdraw money from the ATM on Leggett Street --> ATM TRANSACTIONS
    • The thief called someone for less than a minute asking them to purchase the earliest flight tickects out of
    Fiftyville the next day --> PHONE CALLS AND FLIGHTS */

-- Look at the security footage
SELECT name, phone_number, passport_number
FROM people
    JOIN bakery_security_logs AS logs ON logs.license_plate = people.license_plate
WHERE year = 2023 AND month = 7 AND day = 28
AND hour = 10
AND activity = 'exit';
/*  Initial list of suspects based on the security footage of the bakery parking lot
    +---------+----------------+-----------------+
    |  name   |  phone_number  | passport_number |
    +---------+----------------+-----------------+
    | Vanessa | (725) 555-4692 | 2963008352      |
    | Bruce   | (367) 555-5533 | 5773159633      |
    | Barry   | (301) 555-4174 | 7526138472      |
    | Luca    | (389) 555-5198 | 8496433585      |
    | Sofia   | (130) 555-0289 | 1695452385      |
    | Iman    | (829) 555-5269 | 7049073643      |
    | Diana   | (770) 555-1861 | 3592750733      |
    | Kelsey  | (499) 555-9472 | 8294398571      |
    | Taylor  | (286) 555-6063 | 1988161715      |
    +---------+----------------+-----------------+
*/

-- Look at the ATM transactions and filter suspects further
SELECT DISTINCT people.name, people.phone_number, people.passport_number
FROM people
    JOIN bank_accounts AS accounts ON accounts.person_id = people.id
    JOIN atm_transactions AS transactions ON transactions.account_number = accounts.account_number
    JOIN bakery_security_logs AS logs ON logs.license_plate = people.license_plate
WHERE transactions.transaction_type = 'withdraw'
AND transactions.atm_location LIKE 'Leggett%'
AND logs.year = 2023 AND logs.month = 7 AND logs.day = 28
AND logs.hour = 10 AND logs.minute BETWEEN 15 AND 25
AND logs.activity = 'exit';
/*  Updated list of suspects adding the ATM withdrawals information
    +-------+----------------+-----------------+
    | name  |  phone_number  | passport_number |
    +-------+----------------+-----------------+
    | Bruce | (367) 555-5533 | 5773159633      |
    | Luca  | (389) 555-5198 | 8496433585      |
    | Iman  | (829) 555-5269 | 7049073643      |
    | Diana | (770) 555-1861 | 3592750733      |
    +-------+----------------+-----------------+
*/

-- Check flights for the earliest departure from Fiftyville on July 29
SELECT name, a2.city AS destination
FROM people
    JOIN passengers ON people.passport_number = passengers.passport_number
    JOIN flights ON flights.id = passengers.flight_id
    JOIN airports AS a1 ON a1.id = flights.origin_airport_id
    JOIN airports AS a2 ON a2.id = flights.destination_airport_id
WHERE a1.city = 'Fiftyville'
AND flights.year = 2023 AND flights.month = 7 AND flights.day = 29
AND people.name IN ('Bruce', 'Luca', 'Iman', 'Diana')
ORDER BY hour LIMIT 1;
/*  Final suspect after checking the earliest flight
    +-------+---------------+
    | name  |  destination  |
    +-------+---------------+
    | Bruce | New York City |
    +-------+---------------+
    The THIEF is Bruce and the city the thief ESCAPED TO is New York City
*/

-- Find the accomplice by checking phone call records
SELECT p1.name AS caller, p2.name AS reciever
FROM people AS p1
    JOIN phone_calls AS calls ON p1.phone_number = calls.caller
    JOIN people AS p2 ON calls.receiver = p2.phone_number
WHERE p1.name ='Bruce'
AND calls.year = 2023 AND calls.month = 7 AND calls.day = 28
AND calls.duration < 60;
/* Identified accomplice based on phone call:
    +--------+----------+
    | caller | reciever |
    +--------+----------+
    | Bruce  | Robin    |
    +--------+----------+
    The ACCOMPLICE is Robin
*/
