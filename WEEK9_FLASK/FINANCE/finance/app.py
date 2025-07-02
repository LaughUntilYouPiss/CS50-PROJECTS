import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

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

    # Get user's current id from session
    user_id = session["user_id"]

    # Query database for user's current cash balance
    cash = db.execute(
        "SELECT cash FROM users WHERE id = ?;", user_id
    )[0]["cash"]

    # Query database for user's current stocks
    stocks = db.execute(
        "SELECT symbol, SUM( \
            CASE \
                WHEN direction = 'BUY' THEN shares \
                WHEN direction = 'SELL' THEN -shares \
            END) AS shares \
            FROM transactions \
            WHERE user_id = ? \
            GROUP BY symbol;",
            user_id
    )

    # Keep track of total stock value
    stocks_value = 0
    for stock in stocks:
        price = lookup(stock["symbol"])["price"]        # Look up each stock's current price
        stock["price"] = usd(price)                     # Format price as USD string
        stock["total"] = usd(price * stock["shares"])   # Format total value as USD string
        stocks_value += price * stock["shares"]         # Accumulate total stock value

    # Calculate overall portfolio value
    total = stocks_value + cash

    return render_template("index.html", stocks=stocks, cash=usd(cash), total=usd(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST
    if request.method == "POST":

        # Get user's current id from session
        user_id = session["user_id"]

        # Ensure symbol was submitted
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("missing symbol", 400)

        # Ensure shares are submitted
        shares = request.form.get("shares")
        if not shares:
            return apology("missing shares", 400)

        # Ensure shares exist and valid and lookup their price
        try:
            shares = int(shares)
        except (TypeError, ValueError):
            return apology("invalid shares", 400)
        if shares <= 0:
            return apology("too few shares", 400)

        quote = lookup(symbol)
        if not quote:
            return apology("invalid symbol", 400)

        else:
            # Calculate total cost of purchase
            price = quote["price"]
            product = price * shares

            # Query database for user's available cash
            cash = db.execute(
                "SELECT cash FROM users WHERE id = ?;", user_id
            )[0]["cash"]

            # Check if user can afford the purchase
            if cash < product:
                return apology("can't afford", 400)

            # Deduct the purchase cost from user's cash balance
            db.execute(
                "UPDATE users SET cash = cash - ? WHERE id = ?;", product, user_id
            )

            # Record transaction
            db.execute("INSERT INTO transactions (user_id, symbol, shares, price, direction) \
                        VALUES (?, ?, ?, ?, 'BUY');",
                        user_id, symbol.upper(), shares, price
            )

            # Notify the user of successful purchase and redirect to home page
            flash("Bought!")
            return redirect("/")

    # User reached route via POST
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Get user's current id from session
    user_id = session["user_id"]

    # Query database for user's transactions
    transactions = db.execute(
        "SELECT symbol, \
            CASE \
                WHEN direction = 'BUY' THEN shares \
                WHEN direction = 'SELL' THEN -shares \
            END AS shares, \
            price, transacted \
            FROM transactions WHERE user_id = ?",
        user_id
    )

    # Format prices
    for stock in transactions:
        stock["price"] = usd(stock["price"])

    # Render history
    return render_template("history.html", transactions=transactions)


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

    # User reached route via GET
    if request.method == "GET":
        return render_template("quote.html")

    # User reached route via POST
    else:
        # Ensure symbol was submitted
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("missing symbol", 400)

        # Look up the company name and its current price
        name = lookup(symbol)["name"]
        cost = lookup(symbol)["price"]

        # Render the results page with the quote information
        return render_template("quoted.html", name=name, symbol=symbol, cost=cost)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # User reached route via GET
    if request.method == "GET":
        return render_template("register.html")

    # User reached route via POST
    else:
        username = request.form.get("username")
        password = request.form.get("password")

        # Ensure username was submitted
        if not username:
            return apology("missing username", 400)

        # Ensure password was submitted
        elif not password:
            return apology("missing password", 400)

        # Ensure password was retyped correctly
        elif (
            not request.form.get("confirmation")
            or request.form.get("confirmation") != request.form.get("password")
        ):
            return apology("passwords don't match", 400)

        # Query database for username
        try:
            hash = generate_password_hash(password)
            db.execute(
                "INSERT INTO users (username, hash) VALUES (?, ?)", username, hash
            )
        except ValueError:
            return apology("username taken", 400)

        # Query database for user id
        user = db.execute(
            "SELECT id FROM users WHERE username = ?", username
        )

        # Log the user in
        session["user_id"] = user[0]["id"]

        # Redirect user to home page
        flash("Registered!")
        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST
    if request.method == "POST":

        # Get user's current id from session
        user_id = session["user_id"]

        # Ensure symbol was submitted
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("missing symbol", 400)

        # Ensure shares are submitted
        shares = request.form.get("shares")
        if not shares:
            return apology("missing shares", 400)

        # Ensure shares exist and valid and lookup their price
        try:
            shares = int(shares)
        except (TypeError, ValueError):
            return apology("invalid shares", 400)
        if shares <= 0:
            return apology("too few shares", 400)

        quote = lookup(symbol)
        if not quote:
            return apology("invalid symbol", 400)

        else:
            # Calculate total cost from the sale
            price = quote["price"]
            product = price * shares

            # Update user's cash balance
            db.execute(
                "UPDATE users SET cash = cash + ? WHERE id = ?;", product, user_id
            )

            # Insert transaction record with 'SELL' direction
            db.execute("INSERT INTO transactions (user_id, symbol, shares, price, direction) \
                        VALUES (?, ?, ?, ?, 'SELL');",
                        user_id, symbol.upper(), shares, price
            )

            # Flash confirmation message
            flash("Sold!")
            return redirect("/")

    # User reached route via GET
    else:
        user_id = session["user_id"]
        stocks = db.execute(
            "SELECT DISTINCT symbol FROM transactions WHERE user_id = ?",
            user_id
        )
        return render_template("sell.html", stocks=stocks)
