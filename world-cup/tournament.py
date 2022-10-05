# Simulate a sports tournament

import csv
import sys
import random

# Number of simluations to run
N = 1000


def main():

    # Ensure correct usage
    if len(sys.argv) != 2:
        sys.exit("Usage: python tournament.py FILENAME")

    # TODO: Read teams into memory from file
    teams = get_teams(sys.argv[1])

    # TODO: Simulate N tournaments and keep track of win counts
    counts = run_simulation(teams)

    # Print each team's chances of winning, according to simulation
    for team in sorted(counts, key=lambda team: counts[team], reverse=True):
        print(f"{team}: {counts[team] * 100 / N:.1f}% chance of winning")


def get_teams(filename):
    """Read teams into memory from file. Return the list of teams with ratings"""
    teams = []

    with open(filename) as file:
        reader = csv.DictReader(file)
        for row in reader:
            teams.append(row)
        # convert rating into an int
        for team in teams:
            team["rating"] = int(team["rating"])

    return teams


def simulate_game(team1, team2):
    """Simulate a game. Return True if team1 wins, False otherwise."""
    rating1 = team1["rating"]
    rating2 = team2["rating"]
    probability = 1 / (1 + 10 ** ((rating2 - rating1) / 600))
    return random.random() < probability


def simulate_round(teams):
    """Simulate a round. Return a list of winning teams."""
    winners = []

    # Simulate games for all pairs of teams
    for i in range(0, len(teams), 2):
        if simulate_game(teams[i], teams[i + 1]):
            winners.append(teams[i])
        else:
            winners.append(teams[i + 1])

    return winners


def simulate_tournament(teams):
    """Simulate a tournament. Return name of winning team."""
    # TODO
    teams_qual = teams  # init qualified teams list: this list will reduce along the tournament until getting the winner
    while len(teams_qual) > 1:
        teams_qual = simulate_round(teams_qual)
    return teams_qual[0]["team"]


def run_simulation(teams):
    """Simulate N tournaments and keep track of win counts"""

    # Init counting
    counts = {}
    for team in teams:
        counts[team["team"]] = 0

    # Run simulation N times
    for i in range(N):
        winner = simulate_tournament(teams)
        counts[winner] += 1

    return counts


if __name__ == "__main__":
    main()
