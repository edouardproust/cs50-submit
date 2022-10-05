
-- Main clue: "All you know is that the theft took place on July 28, 2021 and that it took place on Humphrey Street."

--  ====================================================================================
-- /                            Part 1/2: PREPARATION WORK                             /
-- ====================================================================================

-- Let's get familiarized with the database and play around with it.

-- `sqlite> .tables` command gives the list of tables:
-- airports              crime_scene_reports   people
-- atm_transactions      flights               phone_calls
-- bakery_security_logs  interviews
-- bank_accounts         passengers

SELECT * FROM crime_scene_reports
WHERE year = 2021 AND month = 7 AND day = 28;

-- +-----+------+-------+-----+-----------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
-- | id  | year | month | day |     street      |                                                                                                       description                                                                                                        |
-- +-----+------+-------+-----+-----------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
--   ...
-- | 295 | 2021 | 7     | 28  | Humphrey Street | Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery. |
--   ...
-- | 297 | 2021 | 7     | 28  | Humphrey Street | Littering took place at 16:36. No known witnesses.                                                                                                                                                                       |
-- +-----+------+-------+-----+-----------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

SELECT * FROM interviews
WHERE transcript LIKE "%bakery%"
AND (year = 2021 AND month = 7 AND day = 28);

-- +-----+---------+------+-------+-----+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
-- | id  |  name   | year | month | day |                                                                                                                                                     transcript                                                                                                                                                      |
-- +-----+---------+------+-------+-----+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
-- | 161 | Ruth    | 2021 | 7     | 28  | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
-- | 162 | Eugene  | 2021 | 7     | 28  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
-- | 163 | Raymond | 2021 | 7     | 28  | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket. |
-- +-----+---------+------+-------+-----+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

-- Interesting parts:
-- A. "security footage from the bakery parking lot [...] cars that left the parking lot in that time frame"
-- B. "the ATM on Leggett Street [...] withdrawing some money"
-- C. "they called someone [...] for less than a minute [...] earliest flight out of Fiftyville tomorrow"

--> A. Looking in `bakery_security_logs` for exit cars after 10:15am

SELECT * FROM bakery_security_logs
WHERE activity = "exit"
    AND year = 2021
    AND month = 7
    AND day = 28
    AND hour = 10
    AND minute >= 15
    AND minute <= 25;

-- +-----+------+-------+-----+------+--------+----------+---------------+
-- | id  | year | month | day | hour | minute | activity | license_plate |
-- +-----+------+-------+-----+------+--------+----------+---------------+
-- | 260 | 2021 | 7     | 28  | 10   | 16     | exit     | 5P2BI95       |
-- | 261 | 2021 | 7     | 28  | 10   | 18     | exit     | 94KL13X       |
-- | 262 | 2021 | 7     | 28  | 10   | 18     | exit     | 6P58WS2       |
-- | 263 | 2021 | 7     | 28  | 10   | 19     | exit     | 4328GD8       |
-- | 264 | 2021 | 7     | 28  | 10   | 20     | exit     | G412CB7       |
-- | 265 | 2021 | 7     | 28  | 10   | 21     | exit     | L93JTIZ       |
-- | 266 | 2021 | 7     | 28  | 10   | 23     | exit     | 322W7JE       |
-- | 267 | 2021 | 7     | 28  | 10   | 23     | exit     | 0NTHK55       |
-- +-----+------+-------+-----+------+--------+----------+---------------+

--> B. Search in `atm_transactions`

SELECT * FROM atm_transactions
WHERE transaction_type = "withdraw"
    AND atm_location LIKE "%leggett%"
    AND year = 2021
    AND month = 7
    AND day = 28;

-- +-----+----------------+------+-------+-----+----------------+------------------+--------+
-- | id  | account_number | year | month | day |  atm_location  | transaction_type | amount |
-- +-----+----------------+------+-------+-----+----------------+------------------+--------+
-- | 246 | 28500762       | 2021 | 7     | 28  | Leggett Street | withdraw         | 48     |
-- | 264 | 28296815       | 2021 | 7     | 28  | Leggett Street | withdraw         | 20     |
-- | 266 | 76054385       | 2021 | 7     | 28  | Leggett Street | withdraw         | 60     |
-- | 267 | 49610011       | 2021 | 7     | 28  | Leggett Street | withdraw         | 50     |
-- | 269 | 16153065       | 2021 | 7     | 28  | Leggett Street | withdraw         | 80     |
-- | 288 | 25506511       | 2021 | 7     | 28  | Leggett Street | withdraw         | 20     |
-- | 313 | 81061156       | 2021 | 7     | 28  | Leggett Street | withdraw         | 30     |
-- | 336 | 26013199       | 2021 | 7     | 28  | Leggett Street | withdraw         | 35     |
-- +-----+----------------+------+-------+-----+----------------+------------------+--------+

--> C. Search in `airports` and `flights` for the earliest flight (id and time) taking off from Fiftyville ont the 29/7/2021 (day after the theft)
-- And Search in `phone_calls` for the caller and the receiver in that timeframe (call duration shorter than a minute)

SELECT * FROM flights AS f
JOIN airports AS a ON a.id = f.origin_airport_id
WHERE year = 2021
    AND month = 7
    AND day = 29
    AND a.city = "Fiftyville"
ORDER BY hour, minute
LIMIT 1;

-- +----+-------------------+------------------------+------+-------+-----+------+--------+----+--------------+-----------------------------+------------+
-- | id | origin_airport_id | destination_airport_id | year | month | day | hour | minute | id | abbreviation |          full_name          |    city    |
-- +----+-------------------+------------------------+------+-------+-----+------+--------+----+--------------+-----------------------------+------------+
-- | 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     | 8  | CSF          | Fiftyville Regional Airport | Fiftyville |
-- +----+-------------------+------------------------+------+-------+-----+------+--------+----+--------------+-----------------------------+------------+

--> FURTHER INVESTIGATIONS

-- Check phone calls

SELECT * FROM phone_calls AS p
WHERE year = 2021
    AND month = 7
    AND day = 28
    AND duration < 60;

-- +-----+----------------+----------------+------+-------+-----+----------+
-- | id  |     caller     |    receiver    | year | month | day | duration |
-- +-----+----------------+----------------+------+-------+-----+----------+
-- | 221 | (130) 555-0289 | (996) 555-8899 | 2021 | 7     | 28  | 51       |
-- | 224 | (499) 555-9472 | (892) 555-8872 | 2021 | 7     | 28  | 36       |
-- | 233 | (367) 555-5533 | (375) 555-8161 | 2021 | 7     | 28  | 45       |
-- | 251 | (499) 555-9472 | (717) 555-1342 | 2021 | 7     | 28  | 50       |
-- | 254 | (286) 555-6063 | (676) 555-6554 | 2021 | 7     | 28  | 43       |
-- | 255 | (770) 555-1861 | (725) 555-3243 | 2021 | 7     | 28  | 49       |
-- | 261 | (031) 555-6622 | (910) 555-3251 | 2021 | 7     | 28  | 38       |
-- | 279 | (826) 555-1652 | (066) 555-9701 | 2021 | 7     | 28  | 55       |
-- | 281 | (338) 555-6650 | (704) 555-2131 | 2021 | 7     | 28  | 54       |
-- +-----+----------------+----------------+------+-------+-----+----------+

-- Check `passengers` of the flight

SELECT * from passengers
where flight_id = 36;

-- +-----------+-----------------+------+
-- | flight_id | passport_number | seat |
-- +-----------+-----------------+------+
-- | 36        | 7214083635      | 2A   |
-- | 36        | 1695452385      | 3B   |
-- | 36        | 5773159633      | 4A   |
-- | 36        | 1540955065      | 5C   |
-- | 36        | 8294398571      | 6C   |
-- | 36        | 1988161715      | 6D   |
-- | 36        | 9878712108      | 7A   |
-- | 36        | 8496433585      | 7B   |
-- +-----------+-----------------+------+



--  ====================================================================================
-- /                           Part 2/2: PROBLEM RESOLUTION                            /
-- ====================================================================================

-- Now that we are more familirized with the database and the problem, we can solve it more easily.


-- 1. FIND THE THIEF
-- =================

-- So far, we have the following data about the thief:
-- * a list of `bakery_security_logs.license_plate`
-- * a list of `passengers.passport_number`
-- * a list of `phone_calls.caller`
-- * a list of `atm_transactions.account_number`

-- We assume that we can find the identity of the thief if we cross these data

-- bakery_security_logs.license_plate
SELECT name FROM people
WHERE license_plate IN (
    SELECT * FROM bakery_security_logs
    WHERE activity = "exit"
        AND year = 2021
        AND month = 7
        AND day = 28
        AND hour = 10
        AND minute >= 15
        AND minute <= 25
);

-- passengers.passport_number
SELECT name FROM people
WHERE passport_number IN (
    SELECT passport_number FROM passengers
    WHERE flight_id = (
        SELECT f.id FROM flights AS f
        JOIN airports AS a ON a.id = f.origin_airport_id
        WHERE year = 2021
            AND month = 7
            AND day = 29
            AND a.city = "Fiftyville"
        ORDER BY hour, minute
        LIMIT 1
    )
);

-- phone_calls.caller
SELECT name FROM people
WHERE phone_number IN (
SELECT caller FROM phone_calls
WHERE year = 2021
    AND month = 7
    AND day = 28
    AND duration < 60
);

-- atm_transactions.account_number
SELECT p.name FROM people AS p
JOIN bank_accounts AS b ON p.id = b.person_id
WHERE b.account_number IN (
    SELECT account_number FROM atm_transactions
    WHERE transaction_type = "withdraw"
        AND atm_location LIKE "%leggett%"
        AND year = 2021
        AND month = 7
        AND day = 28
);

-- If we WRAP IT ALL TOGETHER we get:

SELECT name FROM people
WHERE
    -- Car licence plates
    name IN (
        SELECT name FROM people
        -- Bakery security logs
        WHERE license_plate IN (
            SELECT license_plate FROM bakery_security_logs
            WHERE activity = "exit"
                AND year = 2021
                AND month = 7
                AND day = 28
                AND hour = 10
                AND minute >= 15
                AND minute <= 25
        )
    )
    -- Passeport number
    AND name IN (
        SELECT name FROM people
        -- Passengers
        WHERE passport_number IN (
            SELECT passport_number FROM passengers
            -- Flights & Airports
            WHERE flight_id = (
                SELECT f.id FROM flights AS f
                JOIN airports AS a ON a.id = f.origin_airport_id
                WHERE year = 2021
                    AND month = 7
                    AND day = 29
                    AND a.city = "Fiftyville"
                ORDER BY hour, minute
                LIMIT 1
            )
        )
    )
    -- Phone numbers
    AND name IN (
        SELECT name FROM people
        -- Phone calls
        WHERE phone_number IN (
            SELECT caller FROM phone_calls
            WHERE year = 2021
                AND month = 7
                AND day = 28
                AND duration < 60
        )
    )
    -- Bank account numbers
    AND name IN (
        SELECT p.name FROM people AS p
        JOIN bank_accounts AS b ON p.id = b.person_id
        -- ATM transactions
        WHERE b.account_number IN (
            SELECT account_number FROM atm_transactions
            WHERE transaction_type = "withdraw"
                AND atm_location LIKE "%leggett%"
                AND year = 2021
                AND month = 7
                AND day = 28
        )
    );

-- +--------+
-- |  name  |
-- +--------+
-- | Bruce  |
-- +--------+

--> THE THIEF IS: Bruce


-- 2. FIND THE ACCOMPLICE
-- ======================

-- This is easy to do now that we know the thief, thanks to the `phone_calls` table

SELECT name FROM people AS p
JOIN phone_calls AS pc ON pc.receiver = p.phone_number
WHERE pc.caller = (
        SELECT phone_number FROM people
        WHERE name = "Bruce"
    )
    AND year = 2021
    AND month = 7
    AND day = 28
    AND duration < 60;

-- +-------+
-- | name  |
-- +-------+
-- | Robin |
-- +-------+

--> THE ACCOMPLICE IS: Robin


-- 3. FIND THE CITY THE THIEF ESCAPED TO
-- =====================================

SELECT a.city FROM airports AS a
JOIN flights AS f ON a.id = f.destination_airport_id
WHERE f.id = (
    SELECT f.id FROM flights AS f
    JOIN airports AS a ON a.id = f.origin_airport_id
    WHERE year = 2021
        AND month = 7
        AND day = 29
        AND a.city = "Fiftyville"
    ORDER BY hour, minute
    LIMIT 1
);

-- +---------------+
-- |     city      |
-- +---------------+
-- | New York City |
-- +---------------+

--> THE DESTINATION CITY OF THE THIEF IS: New York City