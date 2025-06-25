#include <cs50.h>
#include <stdio.h>
#include <string.h>

#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

void sort(pair array[], int left, int right);
void merge(pair array[], int left, int middle, int right);
bool check_cycle(int loser, int winner);

int main(int argc, string argv[])
{
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
            preferences[i][j] = 0;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // LOOK FOR A CANDIDATE CALLED NAME
    for (int i = 0; i < candidate_count; i++)
    {
        // IF FOUND
        if (strcmp(name, candidates[i]) == 0)
        {
            // UPDATE RANKS & RETURN TRUE
            ranks[rank] = i;
            return true;
        }
    }

    // IF NOT FOUND RETURN FALSE
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    // UPDATE PREFERENCES
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            // IF ONE CANDIDATE IS PREFERRED OVER THE OTHER
            if (j != i && preferences[i][j] > preferences[j][i])
            {
                // ADD PAIR TO PAIRS ARRAY
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;

                // UPDATE PAIR COUNT
                pair_count++;
            }
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    sort(pairs, 0, pair_count - 1);
}

void sort(pair array[], int left, int right)
{
    if (left < right)
    {
        int middle = (left + right) / 2;
        sort(array, left, middle);
        sort(array, middle + 1, right);
        merge(array, left, middle, right);
    }
}

void merge(pair array[], int left, int middle, int right)
{
    int l = middle - left + 1, r = right - middle;

    // CREATE TEMPORARY ARRAYS
    pair left_array[l], right_array[r];

    // COPY DATA
    for (int i = 0; i < right; i++)
    {
        left_array[i] = array[i];
        right_array[i] = array[i + middle + 1];
    }

    int i = 0, j = 0, k = left;
    // MERGE
    while (i < l && j < r)
    {
        if (preferences[left_array[i].winner][left_array[i].loser] >=
            preferences[right_array[j].winner][right_array[j].loser])
        {
            array[k] = left_array[i];
            i++;
        }
        else
        {
            array[k] = right_array[j];
            j++;
        }
        k++;
    }

    while (i < l)
    {
        array[k] = left_array[i];
        i++;
        k++;
    }

    while (j < r)
    {
        array[k] = right_array[j];
        j++;
        k++;
    }
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    // CREATE THE LOCKED GRAPH
    for (int i = 0; i < pair_count; i++)
    {
        // IF NOT A CYCLE
        if (!check_cycle(pairs[i].loser, pairs[i].winner))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
}

bool check_cycle(int loser, int winner)
{
    if (loser == winner)
    {
        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i] && check_cycle(i, winner))
        {
            return true;
        }
    }

    return false;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        bool is_source = true;

        // CHECK IF A CANDIDATE LOCKES THE CURRENT CANDIDATE
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true)
            {
                is_source = false;
                break;
            }
        }

        // If NONE PRINT WINNER
        if (is_source)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
}
/*void print_winner(void)
{
    // TODO
    int j = 0;

    // SEARCH FOR THE SOURCE OF THE GRAPH
    for (int i = 0; i < candidate_count; i++)
    {
        for (j = 0; j < candidate_count; j++)
        {
            if (locked[i][j] == true)
            {
                break;
            }
        }

        // IF LOCKED IS FALSE PRINT THE WINNER
        if (j == candidate_count)
        {
            printf("%s\n", candidates[i]);
        }
    }
}
*/
