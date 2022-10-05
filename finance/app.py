import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


DEFAULT_CASH_BALANCE = 10000


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

    user = get_connected_user()
    portfolio = get_portfolio(user)
    [portfolio_with_prices, total_value] = get_portfolio_with_prices(portfolio)
    total_account = total_value + user["cash"]

    # Get request
    return render_template(
        "index.html",
        user=user,
        cash_balance=user["cash"],
        portfolio=portfolio_with_prices,
        total_value=total_value,
        total_account=total_account
    )


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Get data from the form
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Ensure symbol was submitted
        if not symbol:
            return apology("must provide symbol")

        # Ensure shares quantity was submitted
        if not shares:
            return apology("must provide shares quantity")

        # Check if stock exists
        quote = lookup(symbol)
        if not quote:
            return apology("symbol not found")

        # Ensute that shares is a positive integer
        try:
            shares = int(shares)
            if shares < 1:
                return apology("share must be a positive integer")
        except ValueError:
            return apology("share must be a positive integer")

        # Check if user has enough money to buy
        user = get_connected_user()
        total = quote["price"] * shares
        if user["cash"] < total:
            return apology("Not enough cash")

        # Buy stocks, update cahs balance and save transaction
        update_cash_balance(user, total, "B")
        update_portfolio_buy(user, symbol, quote, shares)
        save_transaction(user, "B", symbol, shares, quote)

        # Redirect user to home page
        flash(f"You bought {shares} share(s) of \"{quote['name']}\" ({symbol})")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html", symbol=request.args.get("symbol"))


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    user = get_connected_user()
    transactions = get_transactions(user)
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Get data from the form
        username = request.form.get("username")
        password = request.form.get("password")

        # Ensure username was submitted
        if not username:
            return apology("must provide username")

        # Ensure password was submitted
        elif not password:
            return apology("must provide password")

        # Query database for username
        user = get_user_by_username(username)

        # Ensure username exists and password is correct
        if not user or not check_password_hash(user["hash"], password):
            return apology("invalid username and/or password")

        # Remember which user has logged in
        session["user_id"] = user["id"]

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

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Get values from the form
        symbol = request.form.get("symbol")

        # Ensure username was submitted
        if not symbol:
            return apology("must provide symbol")

        # Get quote
        quote = lookup(symbol)
        if not quote:
            return apology("symbol not found")
        else:
            return render_template("quoted.html", quote=quote)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Get values from the form
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Ensure username was submitted
        if not username:
            return apology("must provide username")

        # Ensure password was submitted
        elif not password:
            return apology("must provide password")

        # Ensure confirmation was submitted
        elif not confirmation:
            return apology("must provide a password confirmation")

        # Ensure password and confirmation match
        elif not confirmation == password:
            return apology("password and confirmation must match")

        # Ensure the username does not already exist in the database
        elif not get_user_by_username(username) == None:
            return apology("username already taken")

        else:
            # Save user into database
            save_user(username, password)

            # Log in & remember the user
            user = get_user_by_username(username)
            session["user_id"] = user["id"]

            # Redirect user to home page
            flash("You have been registered.")
            return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Get data from the form
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Ensure symbol was submitted
        if not symbol:
            return apology("must provide symbol")

        # Ensure shares quantity was submitted
        if not shares:
            return apology("must provide shares quantity")

        # Ensute that shares is a positive integer
        try:
            shares = int(shares)
            if shares < 1:
                return apology("shares must be a positive integer")
        except ValueError:
            return apology("shares must be a positive integer")

        # Check if stock exists
        quote = lookup(symbol)
        if not quote:
            return apology("symbol not found")

        # Check if user has enough shares to sell
        user = get_connected_user()
        if shares > get_shares(user, symbol):
            return apology("Not enough shares")

        # Buy stocks, update cahs balance and save transaction
        total = quote["price"] * shares
        update_cash_balance(user, total, "S")
        update_portfolio_sell(user, symbol, shares)
        save_transaction(user, "S", symbol, shares, quote)

        # Redirect user to home page
        flash(f"You sold {shares} share(s) of \"{quote['name']}\" ({symbol})")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        user = get_connected_user()
        portfolio = get_portfolio(user)
        return render_template("sell.html", portfolio=portfolio, symbol=request.args.get("symbol"))


###################################################################################################
####################################### Query methods #############################################
###################################################################################################


def get_user_by_username(username):
    """ Return username or None. """

    rows = db.execute(
        "SELECT * FROM users WHERE username = ?",
        username
    )
    if len(rows) == 1:
        return rows[0]
    else:
        return None


def get_connected_user():
    user_id = session["user_id"]
    if user_id:
        user = db.execute("SELECT * FROM users WHERE id = ?", user_id)
        if user:
            return user[0]
    else:
        return None


def save_user(username, password):
    db.execute(
        "INSERT INTO users (username, hash) VALUES (?, ?)",
        username, generate_password_hash(password)
    )


def update_cash_balance(user, total, transaction_type):
    query = "UPDATE users SET cash = ? WHERE id = ?"
    if transaction_type == "B":
        return db.execute(query, user["cash"] - total, user["id"])
    elif transaction_type == "S":
        return db.execute(query, user["cash"] + total, user["id"])
    else:
        raise Exception("The transaction type must be either \"B\" (for buy) or \"S\" for (for sell).")


def get_portfolio(user):
    return db.execute(
        "SELECT * FROM portfolios WHERE user_id = ?",
        user["id"]
    )


def get_portfolio_with_prices(portfolio):
    """ Populate the portfolio withthe current price for each stock, and calculate the total value of the portfolio. """
    """ Returns an array: [portfolio_with_prices, total_value] """

    total_value = 0
    for row in portfolio:
        price = lookup(row["symbol"])["price"]
        total_value += price * row["shares"]
        row["price"] = price

    return [portfolio, total_value]


def get_shares(user, symbol):
    rows = db.execute(
        "SELECT shares FROM portfolios WHERE user_id = ? AND symbol = ?",
        user["id"], symbol
    )
    if len(rows) != 1:
        return 0
    return int(rows[0]["shares"])


def update_portfolio_buy(user, symbol, quote, shares):
    shares_before = get_shares(user, symbol)
    # Add row if stock not already in portfolio
    if shares_before == 0:
        return db.execute(
            "INSERT INTO portfolios (user_id, symbol, name, shares) VALUES (?,?,?,?)",
            user["id"], symbol, quote["name"], shares
        )
    # Else update total of shares of the existing row
    else:
        return db.execute(
            "UPDATE portfolios SET shares = ? WHERE symbol = ?", shares_before + int(shares), symbol
        )


def update_portfolio_sell(user, symbol, shares):
    shares_before = get_shares(user, symbol)
    # Update total of shares if amount in portfolio is higher
    if shares_before > shares:
        return db.execute(
            "UPDATE portfolios SET shares = ? WHERE symbol = ?",
            shares_before - int(shares), symbol
        )
    # Else remove row
    else:
        return db.execute(
            "DELETE FROM portfolios WHERE user_id=? AND symbol=?",
            user["id"], symbol
        )


def get_transactions(user):
    return db.execute(
        "SELECT * FROM transactions WHERE user_id = ? ORDER BY created_at DESC",
        user["id"]
    )


def save_transaction(user, type, symbol, shares, quote):
    total = quote["price"] * int(shares)
    return db.execute(
        "INSERT INTO transactions (user_id, type, symbol, name, price, shares, total) VALUES (?,?,?,?,?,?,?)",
        user["id"], type, symbol, quote["name"], quote["price"], shares, total
    )
