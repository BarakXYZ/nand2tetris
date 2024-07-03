////////////

// What is the game loop??

// Maybe should be called from Levels class?
// do UserInterface.printRAM(Characters.getFatherSP());  // ?


// Flow:
// Initialize all params
// Build the UI
// Start Introduction
    // Shimon explains the situation
    // Shimon teaches you how to use the UI
    // Shimon introduces Challenge number 1
    // Player completes Challenge number 1
    // Shimon sends you to the next quest
// Quest 2:
    // Lookup Gini
    // Dialogue with Gini
    // Begin Challenge 2
    // Complete Challenge 2
    // Go back to Shimon
    // Shimon sends you to the next quest
// Quest 3:
    // Lookup...

// Things to keep in mind:

    // We have some range of motion and the player can decide to:

        // Jump back to previous characters

            // We need to deal with those interactions (and dialogues)

            // We need to have a mechanism to block jumping to
            // future characters

            // We need to "lock" a challenge after it's completed

                // So per character challenge with tracking mechanism

// Each character should have states:

    // isQuestPending: will branch to getCurrentQuest or idle

        // getCurrentQuest: will fetch the current quest and
        // branch either startQuest or pendingQuest:

            // isQuestStarted: will branch to either startQuest or

                // startQuest: starts to intro dialogue to the quest

            // pendingQuest

                // pendingQuest: remains idle, pending for the 
                // quest to complete 

        // idle

            // Will repeate an idle dialogue

    // Dependencies of isQuestPending:

        // Maybe isQuestPending is set by the QuestManager?

            // Like 'initQuest2' thingy perhaps?

                // set Gini's param 'isQuestPending' to true

            // Also can init the availability of Gini in general:

                // isReachable: true


                    // isReachable can be probed from the Lookup
                    // to check if an access is allowed and if to draw it

            // If there is more than 1 quest:
                // We can change the index of the currentActiveQuest per 
                // character to the appropriate index, so it will branch
                // properly



Where do I store the conditionals for the quests?
For example, we're at quest 2, and we know Gini should be active only if we're at
2.2.
Where to we place or how do we design that?


initGini?
initShimon?

Or maybe there's just 1 deterministic flow that is given by the QuestManager?

Shimon (0)
Shimon (0.1)
Shimon (0.2)
Shimon (1)
Gini (1.1)
Shimon (1.1) - possibily we can go back to shimon here, so I guess he's pending?
Draco (1.1) - Locked until we're at stage 3 for example?
Something
Shimon
Gini
Shimon
etc.?

Then maybe we can simply advance this quest tree, checking if there's another quest
left todo, if so, advance?

And then in Challenge, when we're sending an answer, we can check if it's the right answer, and if so, we can simply advance to the next quest if there are any more left.

I think we can try to run with a quick demo example and test out this structure.

We essentially need to have some sort of table that holds the dialogue, challenge, etc. of each character in respondense to the current quest.

Maybe we can give an index to each character when they're initialized and later ask via this index the quest manager if there's any dialogue to probe?


What will be the appropriate data structure for QuestManager?
Another option is that every time we look up a character, the character will look
into QuestManager to check what is expected from him at this current stage.
That way everything is managed through QuestManager, and maybe depending on the return value, the character can later call the appropriate dialogue.

do Lookup.address("SHIMON");  // Will check if reachable
do Character.interact();  // Will fetch the currently active character (Shimon)
                       // Will also generate the appropriate dialogue, etc.
function void interact() {
    do Dialogue.get(characterIndex);
}

function void get(int characterIndex) {
    if (QuestManager. 5)
}

function dialogue() {
    
}

// Listen to player's input (LOOP)
    // Lookup
    // Answer (Challenge)
    // Quit
