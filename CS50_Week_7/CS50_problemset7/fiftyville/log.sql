-- Keep a log of any SQL queries you execute as you solve the mystery.

-- See what tables I have to work with.
.tables

-- Get an idea of what info each table offers.
.schema

-- Check the crime reports to hopefully find a lead.
SELECT *
FROM crime_scene_reports
WHERE year = 2024 AND month = 7 AND day = 28 AND street = 'Humphrey Street';
-- NOTES: Found a lead. Duck stolen at 10:15, from the Humphrey Street Bakery. Three witnesses were interviewed and they all mentioned the bakery.

-- Get an idea of what info I can aquire from the interviews.
.schema interviews

-- Get an idea of what info I can aquire from the bakery security logs.
.schema bakery_security_logs

-- Before checking the security logs I want to know exactly what was said during the interviews.
SELECT transcript
FROM interviews
WHERE year = 2024 AND day = 28 AND month = 7;
-- NOTES:
-- The interviews provide critical information.

-- The thief drove away using a car that was parked in the bakery lot within a 10 minute time frame after the theft.
-- He probably left the lot around 10:20 - 10:25.

-- The thief called someone when leaving the bakery. (Probably also around 10:20 - 10:25).
-- During the call (which was less than a minute) the thief said they were planning on taking the earliest flight out of Fiftyville tomorrow (the 29th).
-- The thief asked the person on the phone to buy the plane ticket.

-- The thief withdrew money from an atm on Leggett Street earlier in the day (So could be anytime before 10:15).


-- Check what info I can work with from the bakery security logs again.
.schema bakery_security_logs

-- Check what info I can work with from the bakery security logs.
SELECT *
FROM bakery_security_logs
LIMIT 30;

-- I want to see what types of activity I can specify for in my queries for the bakery logs.
SELECT DISTINCT activity
FROM bakery_security_logs;
-- NOTES: Activity type = entrance or exit.

-- My goal here is to get the license plate. I specified the exact time the thief left the lot, but am unsure whether the lots in the front or back so I just queried for both.
SELECT license_plate
FROM bakery_security_logs
WHERE day = 28 AND month = 7 AND year = 2024 AND hour = 10 AND minute BETWEEN 15 AND 25 AND activity = 'entrance' OR activity = 'exit';
-- NOTES: There were a lot of license plates in the output. Unable to discern which one belongs to the thiefs vehicle.

/* Im going to try to get only the license plates that entered 10 minutes before the crime took place, and left 10 minutes after the crime.
   Im assuming that it took 10 minutes to get to the crime and 10 minutes to get back to the bakery from the crime location. So its safe to assume that
   the thiefs license plate will be in the list of plates that entered between 10:05 and 10:15 and left between 10:15 and 10:25. */
SELECT DISTINCT license_plate
FROM bakery_security_logs
WHERE license_plate IN
    (SELECT license_plate
     FROM bakery_security_logs
     WHERE day = 28 AND month = 7 AND year = 2024 AND hour = 10 AND minute BETWEEN 05 AND 15 AND activity = 'entrance')
AND license_plate IN
    (SELECT license_plate
     FROM bakery_security_logs
     WHERE day = 28 AND month = 7 AND year = 2024 AND hour = 10 AND minute BETWEEN 15 AND 25 AND activity = 'exit');
-- NOTES: This results in nothing meaning no one specific car that entered between 10:05 and 10:15 also left between 10:15 and 10:25, I'll have to rethink my approach.

/* Similar premise to last query instead I check for any cars which parked any time before 10:15 which also left before 10:30. A witness said the thief left within 10 minutes after
   the crime so its safe to assume the thief left before 10:30 and assuming he parked in the lot at some point on the 28th, his license plate is within the table produced
   by this query */
SELECT DISTINCT license_plate
FROM bakery_security_logs
WHERE license_plate IN
    (SELECT license_plate
     FROM bakery_security_logs
     WHERE day = 28 AND month = 7 AND year = 2024 AND hour <= 10 AND minute <= 15 AND activity = 'entrance')
AND license_plate IN
    (SELECT license_plate
     FROM bakery_security_logs
     WHERE day = 28 AND month = 7 AND year = 2024 AND hour >= 10 AND minute >= 15 AND minute <= 30 AND activity = 'exit');
-- NOTES: The license plates in the table made by this query are: 13FNH73, 4328GD8, 5P2BI95, and R3G7486. One of these is most likely the thiefs plate.

-- I want to see what I can work with in the airports table.
.schema airports
-- NOTES: Nothing useful here yet.

-- I want to find what tables have info regarding licenseplates.
.schema
-- NOTES: The tables people has a licen_plate column

-- The thief is one of these individuals.
SELECT name
FROM people
WHERE license_plate IN
    ('13FNH73', '4328GD8', '5P2BI95', 'R3G7486');
-- NOTES: Vanessa, Brandon, Luca, or Sophia.

/* Im going to follow the atm lead next. Witness said she saw the person who stole the duck at the atm on Leggett street earlier that day.
   See what data I have to work with on this. */
.schema bank_accounts
.schema atm_transactions

-- Get a better idea of the atm_transactions table.
SELECT *
FROM atm_transactions LIMIT 50;

/* I want to connect the thief using the atm to his name or at least a list of names. Then I can cross reference these names with the names above. If they match
   then I have a prime suspect. So Im going to select the names from the people table where those peoples ids are in the person_id column of the table bank_accounts
   where the account numbers connected to these bank accounts match to anyone who did a withdraw on Leggett Street on March 28th, 2024. */
SELECT name
FROM people
WHERE id IN
    (SELECT person_id
     FROM bank_accounts
     WHERE account_number IN
        (SELECT account_number
         FROM atm_transactions
         WHERE year = 2024 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw'));
/* NOTES: The names I get from this are Kenny, Iman, Besnista, Taylor, Brooke, Luca, Diana, and Bruce. This makes Luca my prime suspect. Luca was at two places
          witnesses said they saw the thief at. He drove away from the bakery lot at the time the thief did and he did a withdraw from the Leggett Street
          atm where another witness said they saw the thief doing exactly that. What about the other names? Well no other person who did a withdraw from
          the Leggett Street atm also was found leaving the bakery lot around 10:30, about 10 minutes after the crime. Luca is the only person who was at both places as
          the witnesses said the thief was. So I think its safe to say that the thief is Luca. */

-- Now to find out who Lucas accomplice is. I'll start by following the lead provided by one of the witnesses.
-- Get an idea of what I have to work with in phone_calls.
.schema phone_calls
SELECT *
FROM phone_calls
LIMIT 20;

-- Get Lucas phone number.
SELECT phone_number
FROM people
WHERE name = 'Luca';
-- NOTES: Lucas number is 389-555-5198.

-- The witness said the call happened the same day as the crime and that it was less than a minute. Here I query to get an idea of who it could be.
SELECT id, caller, receiver
FROM phone_calls
WHERE year = 2024 AND month = 7 AND day = 28 AND duration <= 60;
-- NOTES: Luca received the call from the number 609-555-5876.

-- Knowing this number, I can simply use the people table to connect it to the numbers owner.
SELECT name
FROM people
WHERE phone_number = '(609) 555-5876';
-- NOTES: Kathryn is the accomplice.

/* Now to find where Luca went. The info I have on this is that on the call Luca asked Kathryn to buy a ticket
   for the earliest flight out of Fiftyville tomorrow (the 29th). So I know that most likely Kathryn purchased the ticket so a lead may be her banking
   info and transactions. The flight is also most likely the earliest which left on the 29th. To start im going to refresh my memory on the info I have to
   work with. */
.schema airports
.schema flights
.schema passengers
.schema bank_accounts
.schema atm_transactions
.schema people
-- NOTES: Bank account info isn't useful here. However everything else is.

-- Here im selecting all flights which left before 12 on the 29th. I want to see when the earliest one left.
SELECT *
FROM flights
WHERE year = 2024 AND month = 7 AND day = 29 AND hour <= 12;
-- NOTES: The earliest flight left at 8:20 AM. That flights id is 36. Origin id is 8. destination id is 4.

/* Having the destination id I can get the location Luca flew to. But I want to confirm Luca is on this flight first. If he is then
   I can know for sure where he's going and it further confirms that he is the thief. */
SELECT name
FROM people
WHERE passport_number IN
    (SELECT passport_number
     FROM passengers
     WHERE flight_id = 36);
/* NOTES: Lucas name is on this list. So Luca confirms all witness accounts. He left from the bakery parking lot within ten minutes of the crime.
          He did a withdraw before the crime at the Legett Street atm. And as a witness heard him discussing on the phone while leaving the bakery,
          he is taking the earliest flight out of Fiftyville. And so I think its safe to assume Luca is the culprit. */

/* Knowing from the previous query above that the destination id is four, and the destination id relates to the id from the airports table, I can use this query
   to find what city Luca escaped to */
SELECT city
FROM airports
WHERE id = 4;
-- NOTES: He went to New York City.

-- I ran check50 and Im wrong. So i'm going to start from the beginning.
/* INTERVIEW NOTES:

-- The thief drove away using a car that was parked in the bakery lot within a 10 minute time frame after the theft.
-- He probably left the lot around 10:20 - 10:25.

-- The thief called someone when leaving the bakery. (Probably also around 10:20 - 10:25).
-- During the call (which was less than a minute) the thief said they were planning on taking the earliest flight out of Fiftyville tomorrow (the 29th).
-- The thief asked the person on the phone to buy the plane ticket.

-- The thief withdrew money from an atm on Leggett Street earlier in the day (So could be anytime before 10:15).
*/

-- Get idea of the info I have to work with from security logs and people.
.schema bakery_security_logs
.schema people

-- Trying to get correct license plate and correlating info.
SELECT name, phone_number, passport_number, people.license_plate
FROM bakery_security_logs
JOIN people ON bakery_security_logs.license_plate = people.license_plate
WHERE year = 2024 AND month = 7 AND day = 28 AND hour = 10 AND minute < 15 AND activity = 'entrance'
AND bakery_security_logs.license_plate IN
   (SELECT bakery_security_logs.license_plate
    FROM bakery_security_logs
    WHERE year = 2024 AND month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute <= 25 AND activity = 'exit');
-- NOTES: This produces nothing. So maybe the robber parked there a day or two prior.

/* Same as last query but adjusted. I made a mistake in my last query as I only checked for people who parked between 10:00 and 10:15.
   I need to check for a car that parked anytime before the crime. */
SELECT name, phone_number, passport_number, people.license_plate
FROM bakery_security_logs
JOIN people ON bakery_security_logs.license_plate = people.license_plate
WHERE year = 2024 AND month = 7 AND day = 28 AND hour < 11 AND activity = 'entrance'
AND bakery_security_logs.license_plate IN
   (SELECT bakery_security_logs.license_plate
    FROM bakery_security_logs
    WHERE year = 2024 AND month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute <= 25 AND activity = 'exit');
/* NOTES: Finally, this one worked. Here are the possible suspects.
+---------+----------------+-----------------+---------------+
|  name   |  phone_number  | passport_number | license_plate |
+---------+----------------+-----------------+---------------+
| Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       |
| Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
| Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
| Kelsey  | (499) 555-9472 | 8294398571      | 0NTHK55       |
| Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
| Vanessa | (725) 555-4692 | 2963008352      | 5P2BI95       |
| Barry   | (301) 555-4174 | 7526138472      | 6P58WS2       |
| Sofia   | (130) 555-0289 | 1695452385      | G412CB7       |
+---------+----------------+-----------------+---------------+
*/

-- Now im going to find who withdrew money from the Leggett atm earlier in the day.
.schema people
.schema atm_transactions
.schema bank_accounts
SELECT name, phone_number, passport_number, license_plate
FROM people
JOIN bank_accounts ON people.id = person_id JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE atm_location = 'Leggett Street' AND year = 2024 AND month = 7 AND day = 28 AND transaction_type = 'withdraw';
/* NOTES: Here are the results:
+---------+----------------+-----------------+---------------+
|  name   |  phone_number  | passport_number | license_plate |
+---------+----------------+-----------------+---------------+
| Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
| Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
| Brooke  | (122) 555-4581 | 4408372428      | QX4YZN3       |
| Kenny   | (826) 555-1652 | 9878712108      | 30G67EN       |
| Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       |
| Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
| Taylor  | (286) 555-6063 | 1988161715      | 1106N58       |
| Benista | (338) 555-6650 | 9586786673      | 8X428L0       |
+---------+----------------+-----------------+---------------+
Bruce, Diana, Iman, and Luca are now the possible suspects. They not only left the bakery within 10 minutes after the crime,
but also did a withdraw from the Leggett Street atm which the thief was said to be doing as well earlier in the day on the 28th. */

/* A witness said that they heard the thief get on a phone call that was less than a minute when leaving the bakery.
   Here I'm going to see if Bruce, Diana, Iman, or Luca made any calls on the 28th which were less than a minute. */
.schema people
.schema phone_calls
SELECT name, phone_number, passport_number, license_plate, receiver
FROM people
JOIN phone_calls ON people.phone_number = phone_calls.caller
WHERE year = 2024 AND month = 7 AND day = 28 AND duration <= 60;
/* NOTES: This query gives the following result:
+---------+----------------+-----------------+---------------+----------------+
|  name   |  phone_number  | passport_number | license_plate |    receiver    |
+---------+----------------+-----------------+---------------+----------------+
| Sofia   | (130) 555-0289 | 1695452385      | G412CB7       | (996) 555-8899 |
| Kelsey  | (499) 555-9472 | 8294398571      | 0NTHK55       | (892) 555-8872 |
| Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       | (375) 555-8161 |
| Kathryn | (609) 555-5876 | 6121106406      | 4ZY7I8T       | (389) 555-5198 |
| Kelsey  | (499) 555-9472 | 8294398571      | 0NTHK55       | (717) 555-1342 |
| Taylor  | (286) 555-6063 | 1988161715      | 1106N58       | (676) 555-6554 |
| Diana   | (770) 555-1861 | 3592750733      | 322W7JE       | (725) 555-3243 |
| Carina  | (031) 555-6622 | 9628244268      | Q12B3Z3       | (910) 555-3251 |
| Kenny   | (826) 555-1652 | 9878712108      | 30G67EN       | (066) 555-9701 |
| Benista | (338) 555-6650 | 9586786673      | 8X428L0       | (704) 555-2131 |
+---------+----------------+-----------------+---------------+----------------+
Bruce, and Diana are now the prime suspects. There actions now fit three witness descriptions. */

/* A witness said that during the phone call the thief asked the receiver to buy him a ticket for the earliest
   flight out of fiftyville on the 29th. I want to see if Bruce and or Diana are on this flight. */
.schema people
.schema airports
.schema flights
.schema passengers
-- Find earliest flight.
SELECT *
FROM flights
WHERE year = 2024 AND month = 7 AND day = 29 AND hour < 12;
-- NOTES: Earliest flight left at 8:20 AM.

SELECT name
FROM people
JOIN passengers ON people.passport_number = passengers.passport_number JOIN flights ON passengers.flight_id = flights.id JOIN airports ON flights.destination_airport_id = airports.id
WHERE flights.year = 2024 AND flights.month = 7 AND flights.day = 29 AND flights.hour = 8 AND flights.minute = 20;
-- NOTES: Diana's not on this flight, only Bruce. I think its safe to say that Bruce is the culprit. He fits all four of the witnesses descriptions.

-- Find where Bruces flight is going.
SELECT city
FROM airports
WHERE id IN
   (SELECT destination_airport_id
    FROM flights
    WHERE year = 2024 AND month = 7 AND day = 29 AND hour = 8 AND minute = 20);
-- NOTES: New York City

-- Find who Bruce's accomplice was.
.schema phone_calls
.schema people
SELECT name
FROM people
WHERE phone_number =
   (SELECT receiver
    FROM phone_calls
    WHERE caller = '(367) 555-5533' AND duration < 60);
-- NOTES: Robin is the accomplice.
