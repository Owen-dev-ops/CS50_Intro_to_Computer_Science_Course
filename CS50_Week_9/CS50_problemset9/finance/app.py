import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, buy_sell

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Get users portfolio.
    user_portfolio = db.execute("""
                                SELECT company, symbol,
                                    SUM(CASE WHEN type = 'buy' THEN shares ELSE 0 END)
                                    - SUM(CASE WHEN type = 'sell' THEN shares ELSE 0 END) AS total_shares
                                FROM history
                                WHERE user_id = ?
                                GROUP BY symbol
                                HAVING
                                    SUM(CASE WHEN type = 'buy' THEN shares ELSE 0 END)
                                    - SUM(CASE WHEN type = 'sell' THEN shares ELSE 0 END) > 0
                                """, session["user_id"])

    # Add total price of stocks owned for each company to user_portfolio dictionary.
    for row in user_portfolio:
        row["total_price"] = row["total_shares"] * lookup(row["symbol"])["price"]

    user_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
    return render_template("index.html", user_portfolio=user_portfolio, user_cash=user_cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")

    # Set order type.
    order_type = "buy"

    # Get form data.
    company_symbol = request.form.get("symbol").upper()
    shares = request.form.get("shares")
    return buy_sell(company_symbol, shares, order_type, db)

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    history = db.execute("SELECT id, symbol, company, shares, price, type, date FROM history WHERE user_id = ?", session["user_id"])
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")

    quote_info = lookup(request.form.get("symbol"))
    if not quote_info:
        return apology("Invalid Company Symbol")
    return render_template("quoted.html", company_name = quote_info["name"], stock_price = quote_info["price"])


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")

    username = request.form.get("username")
    password = request.form.get("password")
    confirmation = request.form.get("confirmation")

    # Make sure password and confirmation were entered.
    if not password or not confirmation:
        return apology("Missing password or confirmation password")

    # Make sure the password and confirmation password match here. if matching continue to username validity check, else return apology page with reason why.
    if password != confirmation:
        return apology("Password and confirm password do not match. Please try again")

    # If request.form.get("username") is empty return apology with reason why.
    if not username:
        return apology("Username Required")

    # Select the id of any users who currently have a username matching the one entered in the register.html form.
    username_validity_check = db.execute("SELECT id FROM users WHERE username = ?", username)
    # If id exist, apologize to the user and inform them that this username is already taken. otherwise continue to open account.
    if username_validity_check:
        return apology("Username already taken")

    # hash password
    password = generate_password_hash(password)

    # Upload new user to users table.
    db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, password)

    # Send user to login page.
    return redirect("/login")

    # JavaScript on registration.html page handles succesful registration message.

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":

        # Create a list of possible stocks the user can sell.
        options = db.execute("""
                             SELECT symbol
                             FROM history
                             WHERE user_id = ?
                             GROUP BY symbol
                             HAVING SUM(CASE WHEN type = 'buy' THEN shares ELSE 0 END)
                             - SUM(CASE WHEN type = 'sell' THEN shares ELSE 0 END) > 0
                             """, session["user_id"])

        return render_template("sell.html", options=options)

    # Set order type.
    order_type = "sell"

    # Get form data.
    company_symbol = request.form.get("symbol")
    shares = request.form.get("shares")
    return buy_sell(company_symbol, shares, order_type, db)

@app.route("/change", methods=["GET", "POST"])
@login_required
def change():
    """Allows user to change password"""

    if request.method == "GET":
        return render_template("change.html")

    new_password = request.form.get("new_password")
    if not new_password:
        return apology("Invalid password")
    new_password = generate_password_hash(new_password)

    db.execute("UPDATE users SET hash = ? WHERE id = ?", new_password, session["user_id"])

    return redirect("/")
