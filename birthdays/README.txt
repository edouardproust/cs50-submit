This web application lets users store and keep track of birthdays.

# Bug fixes

## Submit 3:
- In index(): on POST, replaced `render_template` redundancy by a simple `redirect`
- In edit(): fixed bug on the `data` missing value in jinja (replaced it by `birthday.name`)
- In edit(): on GET, added `ALERTS.clear()` to clean alerts on load

## Submit 2:
- In index(): on POST, `day` and `month` values were reversed(while being saved into database)
- Added methods `get_birthdays` and `get_birthday` for code optimization

# Features

✅ When the / route is requested via GET, the web application displays, in a table, all of the people in the database along with their birthdays.
✅ When the / route is requested via POST, the web application adds a new birthday to the database and then re-render the index page.

The web app also:
✅ Adds the ability to delete and edit birthday entries
✅ Implements a form validation system on each field of the form
✅ Display alerts (error and success messages) on the top the form