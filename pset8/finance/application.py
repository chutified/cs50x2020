import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():

    # get the summed up stocks
    rows = db.execute("SELECT symbol, SUM(shares) AS sum, price FROM deals WHERE user = :userID GROUP BY symbol HAVING sum >= 1", userID=session["user_id"])
    # get user's cash
    userCash = db.execute("SELECT cash FROM users WHERE id = :userID", userID=session["user_id"])[0]["cash"]

    total = userCash
    # get latest prices of the owned stocks
    stockPrices={}
    for row in rows:
        s = row["symbol"]
        l = lookup(s)
        stockPrices[s] = l["price"]
        total += l["price"] * row["sum"]

    return render_template("index.html", stocks=rows, stockPrices=stockPrices, userCash=userCash, total=total)


@app.route("/deposit", methods=["GET", "POST"])
@login_required
def deposit():

    # GET request
    if request.method == "GET":
        return render_template("deposit.html")

    # POST method

    # get variables
    ammount = int(request.form.get("money"))
    add = request.form["action"] == "Deposit"
    if not add:
        ammount = -ammount

    # validate ammount
    budget = db.execute("SELECT cash FROM users WHERE id = :userID", userID = session["user_id"])[0]["cash"]
    budget += ammount
    if budget < 0:
        return apology("Not enough money.", 400)

    # make a transaction
    db.execute("UPDATE users SET cash = cash + :price WHERE id = :userID", price=ammount, userID=session["user_id"])
    return redirect("/")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():

    # GET request
    if request.method == "GET":
        return render_template("buy.html")

    # POST request

    # get values
    symbol = request.form.get("symbol")
    shares = int(request.form.get("shares"))
    resp = lookup(symbol)
    if not symbol or not resp or shares < 1:
        return apology("Invalid values.", 400)

    # get users budget
    rows = db.execute("SELECT * FROM users WHERE id = :userID", userID = session["user_id"])
    userCash = rows[0]["cash"]
    # get total price
    price = shares * resp["price"]

    # make a deal
    if price > userCash:
        return apology("Not enough money.", 403)
    db.execute("UPDATE users SET cash = cash - :price WHERE id = :userID", price=price, userID=session["user_id"])
    db.execute("INSERT INTO deals (user, symbol, shares, price) VALUES (:userID, :symbol, :shares, :price)",
        userID=session["user_id"], symbol=symbol, shares=shares, price=resp["price"])

    return redirect("/")



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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
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

    # GET request
    if request.method == "GET":
        return render_template("quote.html")

    # POST request

    # get variables
    symbol = request.form.get("symbol")
    resp = lookup(symbol)
    if not resp or not symbol:
        return apology("Invalid stock symbol.", 400)

    return render_template("quoted.html", quote=resp)


@app.route("/register", methods=["GET", "POST"])
def register():

    # GET request
    if request.method == "GET":
        return render_template("register.html")

    # POST request

    # get input values
    username = request.form.get("username")
    password = request.form.get("password")
    confirmation = request.form.get("confirmation")

    # validate username
    rows = db.execute("SELECT * FROM users WHERE username = :username", username = username)
    if not username:
        return apology("Username field cannot be blank.", 400)
    elif len(rows) != 0:
        return apology("This username already exists.", 400)

    # validate passwords
    if not password or not confirmation:
        return apology("Password fields cannot be empty.", 400)
    elif password != confirmation:
        return apology("Passwords don't match.", 400)

    # add user's login into the table
    db.execute("INSERT INTO users (username, hash) VALUES (:username, :hashed)", username=username, hashed=generate_password_hash(password))

    return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():

    # GET request
    if request.method == "GET":
        stocks = db.execute("SELECT symbol, SUM(shares) AS sum FROM deals WHERE user = :userID GROUP BY symbol HAVING sum >= 1", userID=session["user_id"])
        return render_template("sell.html", stocks=stocks)

    # get values
    symbol = request.form.get("symbol")
    shares = request.form.get("shares")
    if not shares or not symbol:
        return apology("Fieds can not be empty.", 400)
    shares = int(shares)

    # validate and check symbol
    resp = lookup(symbol)
    if not resp:
        return apology("Invalid symbol.", 400)
    # validate shares
    if shares < 1:
        return apology("Invalid number of shares.", 400)

    # check the value of shares
    stock = db.execute("SELECT SUM(shares) AS sum FROM deals WHERE user = :userID AND symbol = :symbol GROUP BY symbol", userID=session["user_id"], symbol=symbol)[0]
    if shares > stock["sum"] or len(stock) != 1:
        return apology("Not enough stock(s).", 403)

    # sell stocks and earn cash
    db.execute("INSERT INTO deals (user, symbol, shares, price) VALUES (:userID, :symbol, :shares, :price)",
        userID=session["user_id"], symbol=symbol, shares=-shares, price=resp["price"])
    db.execute("UPDATE users SET cash=cash+:price WHERE id = :userID", price=shares*resp["price"], userID=session["user_id"])

    return redirect("/")


@app.route("/history")
@login_required
def history():
    rows = db.execute("SELECT symbol, shares, price, created_at AS time FROM deals WHERE user = :userID ORDER by created_at", userID=session["user_id"])
    print(rows)
    return render_template("history.html", deals=rows)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
