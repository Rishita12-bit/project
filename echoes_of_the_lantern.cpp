#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

struct GameState {
    string name;
    int courage = 2;
    int insight = 2;
    int trust = 2;
    bool hasLanternKey = false;
    bool hasMirrorShard = false;
    bool promisedToRiver = false;
    bool freedArchivist = false;
    vector<string> inventory;
};

string lowerCopy(string text) {
    transform(text.begin(), text.end(), text.begin(), [](unsigned char ch) {
        return static_cast<char>(tolower(ch));
    });
    return text;
}

void pauseForEnter() {
    cout << "\nPress Enter to continue...";
    if (!cin.ignore(numeric_limits<streamsize>::max(), '\n')) {
        cin.clear();
    }
}

void line() {
    cout << "\n------------------------------------------------------------\n";
}

void addItem(GameState& state, const string& item) {
    if (find(state.inventory.begin(), state.inventory.end(), item) == state.inventory.end()) {
        state.inventory.push_back(item);
    }
}

void showStatus(const GameState& state) {
    line();
    cout << state.name << "'s standing\n";
    cout << "Courage: " << state.courage
         << " | Insight: " << state.insight
         << " | Trust: " << state.trust << "\n";

    cout << "Inventory: ";
    if (state.inventory.empty()) {
        cout << "nothing but rainwater and nerve";
    } else {
        for (size_t i = 0; i < state.inventory.size(); ++i) {
            if (i > 0) cout << ", ";
            cout << state.inventory[i];
        }
    }
    cout << "\n";
}

int askChoice(const vector<string>& options) {
    int choice = 0;

    while (true) {
        cout << "\n";
        for (size_t i = 0; i < options.size(); ++i) {
            cout << "  " << (i + 1) << ". " << options[i] << "\n";
        }
        cout << "\nChoose: ";

        if (cin >> choice && choice >= 1 && choice <= static_cast<int>(options.size())) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }

        if (cin.eof()) {
            cout << "\nThe story closes as the input stream falls silent.\n";
            return 1;
        }

        cout << "The night does not understand that answer. Try a number from the list.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void prologue(GameState& state) {
    line();
    cout << "ECHOES OF THE LANTERN\n";
    line();
    cout << "At the edge of the drowned city of Vael, a lighthouse burns with a blue flame.\n";
    cout << "No keeper tends it. No ship can reach it. Yet every midnight it calls one name.\n\n";
    cout << "Tonight, through rain and bells and shuttered windows, it calls yours.\n\n";
    cout << "What is your name? ";
    getline(cin, state.name);
    if (state.name.empty()) {
        state.name = "Rowan";
    }

    cout << "\nThe flame whispers, \"" << state.name << ". Bring back what the city buried.\"\n";
    cout << "You step through the tide gate with three things: a coat, a question, and a pulse that refuses to slow.\n";
    pauseForEnter();
}

void tideGate(GameState& state) {
    line();
    cout << "The Tide Gate\n";
    cout << "A brass gate blocks the flooded road. Beside it, a ferryman in a foxed velvet coat waits in a boat made of black wood.\n";
    cout << "His lantern has no candle. His eyes are bright enough to make up for it.\n\n";
    cout << "\"Payment,\" he says. \"A secret, a song, or a favor owed.\"\n";

    int choice = askChoice({
        "Tell him a true secret about why you came.",
        "Sing the old harbor song your grandmother taught you.",
        "Promise him one favor when the river asks."
    });

    if (choice == 1) {
        cout << "\nYou admit the truth: the lighthouse voice sounded like someone you lost.\n";
        cout << "The ferryman softens. \"Then you know how ghosts bargain.\"\n";
        state.insight += 2;
        state.trust += 1;
    } else if (choice == 2) {
        cout << "\nYour voice shakes at first, then steadies. The river answers with a harmony under the boat.\n";
        cout << "\"A brave noise,\" the ferryman says, and opens the gate.\n";
        state.courage += 2;
    } else {
        cout << "\n\"Done,\" says the ferryman too quickly. A silver ripple twists around your wrist and vanishes.\n";
        cout << "Some promises are hooks with polite names.\n";
        state.promisedToRiver = true;
        state.trust -= 1;
        addItem(state, "River Mark");
    }

    pauseForEnter();
}

void marketOfMurmurs(GameState& state) {
    line();
    cout << "The Market of Murmurs\n";
    cout << "Stalls lean over the water. Their merchants are memories wearing aprons: a laughing baker, a child with pearl buttons, a soldier made of smoke.\n";
    cout << "In the center hangs a locked lantern, its blue fire trapped behind glass.\n\n";
    cout << "A memory-merchant offers three paths to the lantern key.\n";

    int choice = askChoice({
        "Win the key in a riddle contest.",
        "Trade one cherished memory for it.",
        "Distract the market and steal it."
    });

    if (choice == 1) {
        cout << "\nThe merchant asks, \"What grows heavier when shared, yet lighter when named?\"\n";
        cout << "You answer, \"Grief.\"\n";
        cout << "The market falls silent, then applauds with rain on canvas.\n";
        state.insight += 2;
        state.trust += 1;
        state.hasLanternKey = true;
        addItem(state, "Lantern Key");
    } else if (choice == 2) {
        cout << "\nYou surrender the smell of summer bread from a kitchen long gone.\n";
        cout << "The key is cold in your hand. The absence is colder.\n";
        state.hasLanternKey = true;
        state.insight += 1;
        state.courage -= 1;
        addItem(state, "Lantern Key");
    } else {
        cout << "\nYou overturn a bucket of silver eels. While everyone shrieks, you palm the key.\n";
        cout << "The city notices. It always does.\n";
        state.hasLanternKey = true;
        state.courage += 1;
        state.trust -= 2;
        addItem(state, "Lantern Key");
    }

    pauseForEnter();
}

void glassLibrary(GameState& state) {
    line();
    cout << "The Glass Library\n";
    cout << "A library rises from the flood, built from windows salvaged from every house Vael ever lost.\n";
    cout << "Inside, books breathe. Ladders crawl. An archivist is pinned beneath a fallen shelf, fading letter by letter.\n\n";
    cout << "\"The lighthouse is not a tower,\" the archivist gasps. \"It is a lock. Help me, and I can tell you what it holds.\"\n";

    int choice = askChoice({
        "Risk the unstable shelves to free the archivist.",
        "Search the records first before the truth is lost.",
        "Take a mirror shard from the floor and leave quickly."
    });

    if (choice == 1) {
        cout << "\nGlass cuts your palms, but you lift until the shelf groans aside.\n";
        cout << "The archivist reforms into a woman of ink and candlelight. \"Mercy remembers its own.\" \n";
        state.courage += 2;
        state.trust += 2;
        state.freedArchivist = true;
        state.hasMirrorShard = true;
        addItem(state, "Mirror Shard");
    } else if (choice == 2) {
        cout << "\nYou read fast: the blue flame seals the city's drowned heart, a grief-engine built to keep Vael from vanishing completely.\n";
        cout << "When you look up, the archivist has faded, but one bright shard remains.\n";
        state.insight += 3;
        state.trust -= 1;
        state.hasMirrorShard = true;
        addItem(state, "Mirror Shard");
    } else {
        cout << "\nYou pocket a shard that reflects not your face, but your next regret.\n";
        cout << "Behind you, pages whisper your name in disappointment.\n";
        state.courage += 1;
        state.trust -= 2;
        state.hasMirrorShard = true;
        addItem(state, "Mirror Shard");
    }

    pauseForEnter();
}

void riverClaim(GameState& state) {
    if (!state.promisedToRiver) {
        return;
    }

    line();
    cout << "The River Collects\n";
    cout << "Near the lighthouse causeway, the river rises into the shape of a hand.\n";
    cout << "\"Favor owed,\" it says, in the ferryman's borrowed voice. \"Give me your fear. I am hungry.\"\n";

    int choice = askChoice({
        "Give the river your fear and walk on hollow but steady.",
        "Refuse the bargain and fight through the current.",
        "Offer the River Mark back with a better promise: remembrance."
    });

    if (choice == 1) {
        cout << "\nThe river drinks your fear. The world becomes very clear, and very far away.\n";
        state.courage += 3;
        state.insight -= 1;
    } else if (choice == 2) {
        cout << "\nThe current hammers you against drowned stone, but you keep hold of yourself.\n";
        state.courage += 1;
        state.trust -= 1;
    } else {
        cout << "\nYou promise to carry the names of the drowned instead of feeding the river another piece of yourself.\n";
        cout << "The mark dissolves into a string of names around your heart.\n";
        state.insight += 2;
        state.trust += 2;
        state.promisedToRiver = false;
    }

    pauseForEnter();
}

void lighthouse(GameState& state) {
    line();
    cout << "The Blue Lighthouse\n";
    cout << "At the top of the tower, the lantern key turns by itself. The glass opens.\n";
    cout << "Inside the flame floats the drowned heart of Vael: a small, beating star wrapped in chains of memory.\n\n";

    if (state.freedArchivist) {
        cout << "The archivist's voice rises from the mirror shard: \"Choose for the living and the dead. Not for the ache alone.\"\n";
    } else if (state.hasMirrorShard) {
        cout << "The mirror shard shows three reflections: savior, thief, and witness. All of them are you.\n";
    }

    cout << "\nThe flame offers its final bargain.\n";
    int choice = askChoice({
        "Break the chains and release Vael's heart, whatever the cost.",
        "Take the heart's power and become the new keeper.",
        "Listen to the heart before deciding."
    });

    line();
    cout << "Ending\n";

    if (choice == 1) {
        if (state.courage >= 5 && state.trust >= 3) {
            cout << "You break the chains. The city does not vanish; it exhales.\n";
            cout << "Windows glow under the water. Names return to doorways. The dead do not rise, but they are no longer alone.\n";
            cout << "At dawn, Vael becomes an island of bells, and you are remembered as the one who trusted grief to become something gentler.\n";
            cout << "\nEnding: The City Released\n";
        } else {
            cout << "You break the chains with all the strength you have. The heart surges free, too wild, too sudden.\n";
            cout << "Vael becomes mist before sunrise. Beautiful, terrible, gone.\n";
            cout << "You survive with salt on your face and a city-shaped silence in your chest.\n";
            cout << "\nEnding: The Beautiful Ruin\n";
        }
    } else if (choice == 2) {
        if (state.insight >= 5 && state.trust >= 2) {
            cout << "You take the heart, but not as a tyrant. You bind yourself to the lantern and open its light to every wandering soul.\n";
            cout << "Ships begin to see Vael on clear nights: not a warning, but a welcome.\n";
            cout << "You are not free, exactly. But neither are you alone.\n";
            cout << "\nEnding: The Kind Keeper\n";
        } else {
            cout << "Power enters you like winter lightning. The tower bows. The river obeys.\n";
            cout << "By morning, every bell in Vael rings your name, and none of them sound happy.\n";
            cout << "\nEnding: The Crown of Blue Fire\n";
        }
    } else {
        int total = state.courage + state.insight + state.trust;
        if (state.hasLanternKey && state.hasMirrorShard && total >= 10) {
            cout << "You listen. Beneath the flame is not a command, but a chorus: the drowned asking to be carried, not kept.\n";
            cout << "You set the heart into the mirror shard. It splits into a thousand small lights, each finding a home in someone willing to remember.\n";
            cout << "Vael remains half-flooded, half-waking, entirely changed. So are you.\n";
            cout << "\nEnding: A Thousand Lanterns\n";
        } else {
            cout << "You listen, but too many voices have been lost along the way. The heart's song collapses into a single note.\n";
            cout << "You leave the lighthouse as it was, blue and burning, with one new name in its midnight call.\n";
            cout << "\nEnding: The Unanswered Bell\n";
        }
    }
}

bool playAgain() {
    cout << "\nWould you like to play again? (yes/no): ";
    string answer;
    if (!getline(cin, answer)) {
        return false;
    }
    answer = lowerCopy(answer);
    return answer == "yes" || answer == "y";
}

int main() {
    cout << "Welcome. This story responds to your choices; there are several endings.\n";

    do {
        GameState state;
        prologue(state);
        tideGate(state);
        showStatus(state);
        pauseForEnter();
        marketOfMurmurs(state);
        showStatus(state);
        pauseForEnter();
        glassLibrary(state);
        showStatus(state);
        pauseForEnter();
        riverClaim(state);
        showStatus(state);
        pauseForEnter();
        lighthouse(state);
    } while (playAgain());

    cout << "\nThe lantern dims. Somewhere, water remembers your footsteps.\n";
    return 0;
}
