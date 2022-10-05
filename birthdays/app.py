import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

ALERTS = []


# Routes ====================================================


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        # TODO: Add the user's entry into the database
        # Validate form
        [name, day, month] = validate_form(request)
        # Add entry to database
        if len(ALERTS) == 0:
            db.execute("INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?)", name, month, day)
            ALERTS.append(["success", name + "'s birthday added."])
        # Response
        return redirect("/")
    else:
        # TODO: Display the entries in the database on index.html
        return render_template(
            "index.html",
            birthdays = get_birthdays(),
            alerts = ALERTS
        )


@app.route("/edit", methods=["GET", "POST"])
def edit():
    id = request.args.get("id")
    if id:
        if request.method == "POST":
            # Validate form
            [name, day, month] = validate_form(request)
            if len(ALERTS) == 0:
                # Add entry to database
                db.execute("UPDATE birthdays SET name = ?, day = ?, month = ? WHERE id = ?", name, day, month, id)
                ALERTS.append(["success", name + "'s birthday updated."])
                # Response
                return redirect("/")
            else:
                # Response (refresh)
                return redirect("/edit?id=" + id)
        else:
            ALERTS.clear()
            return render_template(
                "edit.html",
                birthday = get_birthday(id),
                birthdays = get_birthdays(),
                alerts = ALERTS
            )
    else:
        # Response (redirect + error alert)
        ALERTS.append(["danger", "An id must be provided to edit a birthday."])
        return redirect("/")


@app.route("/delete", methods=["POST"])
def delete():
    # Handle alerts
    ALERTS.clear()
    # Delete entry
    id = request.form.get("id")
    if id:
        birthday = get_birthday(id)
        db.execute("DELETE FROM birthdays WHERE id = ?", id)
        ALERTS.append(["success", birthday["name"] + "'s birthday deleted."])
    # Response
    return redirect("/")


# Helpers ====================================================


def get_birthdays():
    return db.execute("SELECT * FROM birthdays")

def get_birthday(id):
    data = db.execute("SELECT * FROM birthdays WHERE id = ?", id)
    return data[0]


def validate_form(request):
    # Define vars
    name = request.form.get("name")
    day = request.form.get("day")
    month = request.form.get("month")
    # Check for alerts on vars
    ALERTS.clear()
    if len(name) == 0: ALERTS.append(["danger", "Name can't be empty"])
    if len(day) == 0 : ALERTS.append(["danger", "Day can't be empty"])
    if len(month) == 0: ALERTS.append(["danger", "Month can't be empty"])
    if len(day) > 0 and (int(day) < 1 or int(day) > 31): ALERTS.append(["danger", "Day must be a number between 1 and 31"])
    if len(month) > 0 and (int(month) < 1 or int(month) > 12): ALERTS.append(["danger", "Month must be a number between 1 and 12"])
    # Return vars
    return [name, day, month]