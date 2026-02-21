import requests

from flask import redirect, render_template, session
from functools import wraps
from datetime import date, datetime


def apology(message, code=400):
    """Render message as an apology to user."""

    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [
            ("-", "--"),
            (" ", "-"),
            ("_", "__"),
            ("?", "~q"),
            ("%", "~p"),
            ("#", "~h"),
            ("/", "~s"),
            ('"', "''"),
        ]:
            s = s.replace(old, new)
        return s

    return render_template("apology.html", top=code, bottom=escape(message)), code


def login_required(f):
    """
    Decorate routes to require login.

    https://flask.palletsprojects.com/en/latest/patterns/viewdecorators/
    """

    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)

    return decorated_function


def lookup(symbol):
    """Look up quote for symbol."""
    url = f"https://finance.cs50.io/quote?symbol={symbol.upper()}"
    try:
        response = requests.get(url)
        response.raise_for_status()  # Raise an error for HTTP error responses
        quote_data = response.json()
        return {
            "name": quote_data["companyName"],
            "price": quote_data["latestPrice"],
            "symbol": symbol.upper()
        }
    except requests.RequestException as e:
        print(f"Request error: {e}")
    except (KeyError, ValueError) as e:
        print(f"Data parsing error: {e}")
    return None


def usd(value):
    """Format value as USD."""
    return f"${value:,.2f}"



def buy_sell(symbol, shares, type, db):
    """Process a buy or sell transaction."""

    # Verify form fields arent blank.
    if not symbol:
        return apology("Invalid company symbol")
    elif not shares.isdigit() or shares is None or int(shares) == 0:
        return apology("Invalid number of shares")

    # Aquire the name of the company, the price, and the date of this order as well as how much cash this user has in their account.
    company_info = lookup(symbol)

    # Verify symbol was valid
    if company_info is None:
        return apology("Invalid company symbol")

    company_name = company_info["name"]
    price = float(shares) * company_info["price"]
    transaction_date = datetime.now()
    user_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

    # If order type is buy process a buy order.
    if type == "buy":
        if price > user_cash:
            return apology("Insufficient funds")

        # Upload purchase to history table.
        db.execute("INSERT INTO history (symbol, company, shares, price, type, date, user_id) VALUES (?, ?, ?, ?, ?, ?, ?)", symbol, company_name, shares, price, type, transaction_date, session["user_id"])

        # Update how much cash the user has.
        user_cash = user_cash - price

    # Else if the order type is sell process a sell order.
    else:
        # Calculate if user owns enough shares to complete this sell order.
        total_owned = db.execute("""SELECT
                                    SUM(CASE WHEN type = "buy" THEN shares else 0 END)
                                    -
                                    SUM(CASE WHEN type = "sell" THEN shares else 0 END)
                                    AS owned
                                 FROM history
                                 WHERE symbol = ? AND user_id = ?
                                 """, symbol, session["user_id"])[0]["owned"]

        if int(shares) > int(total_owned):
            return apology("You don't own this many shares")

        # Upload purchase to history table.
        db.execute("INSERT INTO history (symbol, company, shares, price, type, date, user_id) VALUES (?, ?, ?, ?, ?, ?, ?)", symbol, company_name, shares, price, type, transaction_date, session["user_id"])

        # Update how much cash the user has.
        user_cash = user_cash + price

    # Update how much cash the user has in their account.
    db.execute("UPDATE users SET cash = ? WHERE id = ?", user_cash, session["user_id"])

    return redirect("/")
