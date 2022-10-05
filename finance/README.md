# Run on Flask

**Run the following command in the terminal:**

```bash
export API_KEY={{value}}
flask run
```
replacing `{{value}}` by the API token on https://iexcloud.io/console/tokens (starting by "pk_")

# Check

**To check the code styling of all files, run this command:
```bash
style50 *.py
```

**To test that the app works properly, run the following command:**
```bash
check50 cs50/problems/2022/x/finance
```

**List of criterias in check50:**

- app.py exists
- application starts up

*REGISTER*
- register page has all required elements
- registering user succeeds
- registration with an empty field fails
- registration with password mismatch fails
- registration rejects duplicate username

*LOGIN*
- login page has all required elements
- logging in as registered user succceeds
- quote page has all required elements
- quote handles invalid ticker symbol
- quote handles blank ticker symbol
- quote handles valid ticker symbol

*BUY*
- buy page has all required elements
- buy handles invalid ticker symbol
- buy handles fractional, negative, and non-numeric shares
- buy handles valid purchase

*SELL*
- sell page has all required elements
- sell handles invalid number of shares
- sell handles valid sale
