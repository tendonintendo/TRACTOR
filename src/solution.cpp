#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
using namespace std;
typedef long long ll;

typedef struct {
  char rank, suit;
  int val;
} Card;

typedef struct {
  int qty;
  int spades, clubs, hearts, diamonds;
  string name;
  bool isTrump;
} Player;

typedef struct {
  int tractorLengths[30];
  int tractorCount;
  int pairs;
  int singles;
} ThrowStructure;

char mainSuit, CR1, CR2, CR;
Player players[5];
Card* winnerThrow;
ll hidden = 200LL;
int trick, leaderIdx, FT, CT, played;
ll FTpts, maxLen;

int precedence[256] = {0};

void init_precedence() {
    string ranks = "23456789TJQKA"; 
    for (int i = 0; i < ranks.size(); ++i) {
        precedence[(unsigned char)ranks[i]] = i + 1;
    }
}

inline int getPrecedence(char rank) {
    return precedence[(unsigned char)rank];
}

int cardValue(const Card& card) {
  if (mainSuit == 'O') {
    if (card.suit == 'R') return 16;
    if (card.suit == 'B') return 15;
    if (card.rank == CR) return 14;
  } else {
    if (card.suit == 'R') return 28;
    if (card.suit == 'B') return 27;
    if (card.suit == mainSuit && card.rank == CR) return 26;
    if (card.rank == CR) return 25;
    if (card.suit == mainSuit) {
      int p = getPrecedence(card.rank);
      int pCR = getPrecedence(CR);
      return p + 12 - (p > pCR ? 1 : 0);
    }
  }
  int p = getPrecedence(card.rank);
  int pCR = getPrecedence(CR);
  if (p > pCR) return p - 1;
  return p;
}

bool sameSuits(const Player& a, const Player& b) {
  return a.spades == b.spades && a.clubs == b.clubs && a.hearts == b.hearts && a.diamonds == b.diamonds;
}

bool isTrumpCard(const Card& card) {
  return (card.suit == 'R' || card.suit == 'B' || card.suit == mainSuit || card.rank == CR);
}

bool isPair(const Card& a, const Card& b) {
  return (a.suit == b.suit && a.rank == b.rank);
}

bool cardCompare(const Card& a, const Card& b) { return a.val < b.val; }

ThrowStructure getStructure(Card cards[], int n, int lead) {
  ThrowStructure ts;
  ts.tractorCount = 0;
  ts.pairs = 0;
  ts.singles = 0;
  static bool usedArr[30];
  for (int i = 0; i < 30; ++i) usedArr[i] = false;

  for (int len = n; len >= 4; len -= 2) {
    for (int i = 0; i <= n - len; ++i) {
      if (usedArr[i]) continue;
      bool valid = true;
      for (int j = 0; j < len; j += 2) {
        if (usedArr[i + j] || !isPair(cards[i + j], cards[i + j + 1])) {
          valid = false;
          break;
        }
        if (j >= 2) {
          if (!isTrumpCard(cards[i + j]) &&
              cards[i + j].suit != cards[i + j - 2].suit) {
            valid = false;
            break;
          }
          if (cards[i + j].val != cards[i + j - 2].val + 1) {
            valid = false;
            break;
          }
        }
      }
      if (!valid) continue;
      if (lead && len > maxLen) maxLen = len;
      ts.tractorLengths[ts.tractorCount++] = len;
      for (int j = 0; j < len; ++j) usedArr[i + j] = true;
    }
  }

  for (int i = 0; i < n - 1; ++i) {
    if (!usedArr[i] && !usedArr[i + 1] && isPair(cards[i], cards[i + 1])) {
      ts.pairs++;
      usedArr[i] = usedArr[i + 1] = true;
      if (lead && 2 > maxLen) maxLen = 2;
    }
  }

  for (int i = 0; i < n; ++i) {
    if (!usedArr[i]) {
      ts.singles++;
      if (lead && 1 > maxLen) maxLen = 1;
    }
  }
  return ts;
}

bool sameStructure(const ThrowStructure& lead, const ThrowStructure& follow) {
  int totalBlocks = lead.tractorCount + lead.pairs + lead.singles;
  static int structure[30];
  int idx = 0;

  for (int i = 0; i < lead.tractorCount; ++i) {
    structure[idx++] = lead.tractorLengths[i];
  }
  for (int i = 0; i < lead.pairs; ++i) {
    structure[idx++] = 2;
  }
  for (int i = 0; i < lead.singles; ++i) {
    structure[idx++] = 1;
  }

  if (totalBlocks == 0) return true;
  sort(structure, structure + totalBlocks);
  
  do {
    int fPairs = follow.pairs * 2;
    int fSingles = follow.singles;
    static int tractorParts[30];

    for (int j = 0; j < follow.tractorCount; ++j) {
      tractorParts[j] = follow.tractorLengths[j];
    }
    for (int j = follow.tractorCount; j < lead.tractorCount; ++j) {
      tractorParts[j] = 0;
    }

    bool match = true;
    for (int k = 0; k < totalBlocks; ++k) {
      int need = structure[k];
      if (need >= 4) {
        bool found = false;
        for (int j = 0; j < follow.tractorCount; ++j) {
          if (tractorParts[j] >= need) {
            tractorParts[j] -= need;
            found = true;
            break;
          }
        }
        if (!found) {
          match = false;
          break;
        }
      } else if (need == 2) {
        if (fPairs > 0) {
          fPairs -= 2;
        } else {
          bool used2 = false;
          for (int j = 0; j < follow.tractorCount; ++j) {
            if (tractorParts[j] >= 2) {
              tractorParts[j] -= 2;
              used2 = true;
              break;
            }
          }
          if (!used2) {
            match = false;
            break;
          }
        }
      } else {
        if (fSingles > 0) {
          fSingles--;
        } else if (fPairs > 0) {
          fPairs--;
        } else {
          bool used1 = false;
          for (int j = 0; j < follow.tractorCount; ++j) {
            if (tractorParts[j] >= 1) {
              tractorParts[j] -= 1;
              used1 = true;
              break;
            }
          }
          if (!used1) {
            match = false;
            break;
          }
        }
      }
    }
    if (match) return true;
  } while (next_permutation(structure, structure + totalBlocks));
  return false;
}

int evaluateThrow(Card leads[], int n, Card follows[4][30], Card* bestCard, int* winnerIdx) {
  int relativeIdx = 0;
  sort(leads, leads + n, cardCompare);
  ThrowStructure leadStr = getStructure(leads, n, 1);
  *bestCard = leads[0];
  *winnerIdx = leaderIdx;
  int countGroups = (leadStr.singles > 0) + (leadStr.pairs > 0) + (leadStr.tractorCount > 0);

  bool isThrow = (leadStr.singles > 1) || (leadStr.pairs > 1) || (leadStr.tractorCount > 1) || (countGroups > 1);

  if (players[leaderIdx].isTrump && isThrow) {
    return relativeIdx;
  }

  for (int i = 1; i < 4; ++i) {
    int idx = (i + leaderIdx - 1) % 4 + 1;
    Player followPlayer = players[idx];
    Card challenger = follows[i][0];

    if (!followPlayer.isTrump &&
        (!sameSuits(players[leaderIdx], followPlayer) || challenger.val > 13 ||
         (!players[leaderIdx].isTrump && isThrow))) {
      continue;
    }

    sort(follows[i], follows[i] + n, cardCompare);
    ThrowStructure fs = getStructure(follows[i], n, 0);

    if (!sameStructure(leadStr, fs)) continue;

    bool used[30];
    for (int k = 0; k < n; ++k) used[k] = false;
    int bestHonor = -1, bestLen = 0;

    for (int t = 0; t < leadStr.tractorCount; ++t) {
      int len = leadStr.tractorLengths[t];
      bool found = false;
      for (int j = 0; j <= n - len; ++j) {
        bool valid = true;
        for (int k = 0; k < len; k += 2) {
          if (used[j + k] || used[j + k + 1] ||
              !isPair(follows[i][j + k], follows[i][j + k + 1])) {
            valid = false;
            break;
          }
          if (k >= 2) {
            if (!isTrumpCard(follows[i][j + k]) &&
                follows[i][j + k].suit != follows[i][j + k - 2].suit) {
              valid = false;
              break;
            }
            if (follows[i][j + k].val != follows[i][j + k - 2].val + 1) {
              valid = false;
              break;
            }
          }
        }
        if (!valid) continue;
        int honor = follows[i][j + len - 2].val;
        if (len > bestLen || (len == bestLen && honor > bestHonor)) {
          bestLen = len;
          bestHonor = honor;
        }
        for (int k = 0; k < len; ++k) used[j + k] = true;
        found = true;
        break;
      }
      if (!found) break;
    }

    for (int p = 0; p < leadStr.pairs; ++p) {
      for (int j = 0; j < n - 1; ++j) {
        if (!used[j] && !used[j + 1] &&
            isPair(follows[i][j], follows[i][j + 1])) {
          int honor = follows[i][j].val;
          if (2 > bestLen || (2 == bestLen && honor > bestHonor)) {
            bestLen = 2;
            bestHonor = honor;
          }
          used[j] = used[j + 1] = true;
          break;
        }
      }
    }

    for (int s = 0; s < leadStr.singles; ++s) {
      for (int j = 0; j < n; ++j) {
        if (!used[j]) {
          int honor = follows[i][j].val;
          if (1 > bestLen || (1 == bestLen && honor > bestHonor)) {
            bestLen = 1;
            bestHonor = honor;
          }
          used[j] = true;
          break;
        }
      }
    }

    if (bestHonor > bestCard->val) {
      Card c;
      c.rank = 'X';
      c.suit = 'X';
      c.val = bestHonor;
      *bestCard = c;
      relativeIdx = i;
      *winnerIdx = idx;
    }
  }
  return relativeIdx;
}

char scoreRank(ll x) {
  if (x == 9)
    return 'T';
  else if (x == 10)
    return 'J';
  else if (x == 11)
    return 'Q';
  else if (x == 12)
    return 'K';
  else if (x == 13)
    return 'A';
  else
    return char('1' + x);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(NULL);
  ll t;
  cin >> t;
  players[1].name = "Alice";
  players[2].name = "Bob";
  players[3].name = "Charles";
  players[4].name = "David";
	init_precedence();
  string leaderName;
  ll caseNo = 1;

  while (t--) {
    cout << "Case #" << caseNo++ << ":\n";
    players[1].qty = players[2].qty = players[3].qty = players[4].qty = 0;
    hidden = 200LL;
    FTpts = 0LL;
    trick = 1;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin >> mainSuit >> leaderName >> CR1 >> CR2;
    leaderIdx = leaderName[0] - 'A' + 1;
    CT = (leaderIdx % 2 != 0) ? 1 : 2;
    FT = (CT == 2) ? 1 : 2;
    CR = (leaderIdx % 2 != 0) ? CR1 : CR2;

    while (players[leaderIdx].qty < 25) {
      maxLen = 0;
      players[leaderIdx].isTrump = true;
      played = 0;
      ll pts = 0;
      players[1].clubs = players[2].clubs = players[3].clubs =
          players[4].clubs = 0;
      players[1].hearts = players[2].hearts = players[3].hearts =
          players[4].hearts = 0;
      players[1].spades = players[2].spades = players[3].spades =
          players[4].spades = 0;
      players[1].diamonds = players[2].diamonds = players[3].diamonds =
          players[4].diamonds = 0;
      string blobs[4];
      Card leads[30];
      Card follows[4][30];

      for (int i = 0; i < 4; ++i) {
        cin >> blobs[i];
      }
      int len = blobs[0].length();
      int cnt = 0;
      for (int i = 0; i < len; i += 2) {
        Card card;
        card.suit = blobs[0][i];
        card.rank = blobs[0][i + 1];
        card.val = 0;
        card.val = cardValue(card);
        if (card.rank == '5') pts += 5;
        if (card.rank == 'T' || card.rank == 'K') pts += 10;
        if (card.suit == 'S') players[leaderIdx].spades++;
        if (card.suit == 'C') players[leaderIdx].clubs++;
        if (card.suit == 'H') players[leaderIdx].hearts++;
        if (card.suit == 'D') players[leaderIdx].diamonds++;
        players[leaderIdx].qty++;
        leads[cnt] = card;
        cnt++;
        if (players[leaderIdx].isTrump) {
          players[leaderIdx].isTrump = isTrumpCard(card);
        }
      }

      played = cnt;
      for (int i = 1; i < 4; ++i) {
        int idx = (i + leaderIdx - 1) % 4 + 1;
        players[idx].isTrump = true;
        string& blob = blobs[i];
        int lenf = blob.length();
        cnt = 0;
        for (int j = 0; j < lenf; j += 2) {
          Card card;
          card.suit = blob[j];
          card.rank = blob[j + 1];
          card.val = 0;
          card.val = cardValue(card);
          if (card.rank == '5') pts += 5;
          if (card.rank == 'T' || card.rank == 'K') pts += 10;
          if (card.suit == 'S') players[idx].spades++;
          if (card.suit == 'C') players[idx].clubs++;
          if (card.suit == 'H') players[idx].hearts++;
          if (card.suit == 'D') players[idx].diamonds++;
          players[idx].qty++;
          follows[i][cnt] = card;
          cnt++;
          if (players[idx].isTrump) {
            players[idx].isTrump = isTrumpCard(card);
          }
        }
      }

      Player leaderP = players[leaderIdx];
      int winnerIdx = leaderIdx;
      Card bestCard;
      int relativeIdx =
          evaluateThrow(leads, played, follows, &bestCard, &winnerIdx);
      if (relativeIdx == 0) {
        winnerThrow = leads;
      } else {
        winnerThrow = follows[relativeIdx];
      }
      leaderIdx = winnerIdx;
      if (leaderIdx % 2 == FT % 2) {
        FTpts += pts;
      }
      hidden -= pts;
      trick++;
    }

    if (leaderIdx % 2 == FT % 2) {
      FTpts += hidden * (1LL << maxLen);
    }
    ll CTgain = 0, FTgain = 0;
    if (FTpts == 0) {
      CTgain = 3;
    } else if (FTpts < 40) {
      CTgain = 2;
    } else if (FTpts < 80) {
      CTgain = 1;
    } else if (FTpts > 80) {
      ll k = (FTpts - 80) / 40;
      FTgain += k;
    }

    ll nextCR1, nextCR2;
    if (CT == 1) {
      nextCR1 = (ll)getPrecedence(CR1) + CTgain;
      nextCR2 = (ll)getPrecedence(CR2) + FTgain;
    } else {
      nextCR2 = (ll)getPrecedence(CR2) + CTgain;
      nextCR1 = (ll)getPrecedence(CR1) + FTgain;
    }

    cout << FTpts << "\n";
    if (nextCR1 > 13) {
      cout << "Winner: Team 1";
    } else if (nextCR2 > 13) {
      cout << "Winner: Team 2";
    } else {
      int nextTeam = (FTpts < 80 ? 2 : 1);
      cout << scoreRank(nextCR1) << " " << scoreRank(nextCR2) << " "
           << players[(leaderName[0] - 'A' + nextTeam) % 4 + 1].name;
    }
    cout << "\n";
  }
  return 0;
}