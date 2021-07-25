
$word_list = @(
    "was", "are", "have", "had", "were", "can", "said", "use", "will", "would",
    "make", "like", "has", "look", "write", "see", "could", "been", "call",
    "find", "did", "get", "come", "made", "may", "take", "know", "live",
    "give", "think", "say", "help", "tell", "follow", "came", "want", "show",
    "set", "put", "does", "must", "ask", "went", "read", "need", "move", "try",
    "change", "play", "spell", "found", "study", "learn", "should", "add",
    "keep", "start", "thought", "saw", "turn", "might", "close", "seem",
    "open", "begin", "got", "run", "walk", "began", "grow", "took", "carry",
    "hear", "stop", "miss", "eat", "watch", "let", "cut", "talk", "being", 
    "leave", "word", "time", "number", "way", "people", "water", "day", "part",
    "sound", "work", "place", "year", "back", "thing", "name", "sentence",
    "man", "line", "boy", "farm", "end", "men", "land", "home", "hand",
    "picture", "air", "animal", "house", "page", "letter", "point", "mother",
    "answer", "america", "world", "food", "country", "plant", "school",
    "father", "tree", "city", "earth", "eye", "head", "story", "example", 
    "life", "paper", "group", "children", "side", "feet", "car", "mile", 
    "night", "sea", "river", "state", "book", "idea", "face", "indian", "girl",
    "mountain", "list", "song", "family", "one", "all", "each", "other", "many",
    "some", "two", "more", "long", "new", "little", "most", "good", "great",
    "right", "mean", "old", "any", "same", "three", "small", "another", "large",
    "big", "even", "such", "different", "kind", "still", "high", "every", "own",
    "light", "left", "few", "next", "hard", "both", "important", "white", "four",
    "second", "enough", "above", "young", "not", "when", "there", "how", "out", 
    "then", "first", "now", "only", "very", "just", "where", "much", "before", 
    "too", "also", "around", "well", "here", "why", "again", "off", "away", 
    "near", "below", "last", "never", "always", "together", "often", "once", 
    "later", "far", "really", "almost", "sometimes", "soon", "for", "with", "from", 
    "about", "into", "down", "over", "after", "through", "between", "under", 
    "along", "until", "without", "you", "that", "his", "they", "this", "what", 
    "your", "which", "she", "their", "them", "these", "her", "him", "who", 
    "its", "our", "something", "those", "and", "but", "than", "because", "while" )
               
function Get-RandWords ($list, $num) {
    $rand_words = @()
    for($i = 0; $i -lt $num; $i++) {
        $rand_words += (get-random -InputObject $list)
    }
    return $rand_words
}
function Write-Lines ($lines) {
    foreach ($line in $lines) {
        foreach ($word in $line) {
            if ($word -is [string]) {
                write-host $word -NoNewline
                write-host " " -NoNewline
            } else {
                write-host $word[0] -NoNewline -ForegroundColor $word[1]
                write-host " " -NoNewline
            }
        }
        write-host ""
    }
}1
function Write-Char ($x, $y, $letter, $master_string, $color) {
    $host.UI.RawUI.CursorPosition = @{ x = $x; y = $y }
    if ($color) {
        Write-Host $letter -ForegroundColor $color -NoNewline
    } elseif ($letter -eq $master_string[$PC]) {
        Write-Host $letter -ForegroundColor Yellow -NoNewline
    } else {
        Write-Host $letter -ForegroundColor Red -NoNewline
    }
}

$words_per_line = 15
$PC = 0
$Y = 0
$num_right_words = 0
$num_wrong_words = 0

Clear-Host

$line1 = Get-RandWords $word_list $words_per_line
$line2 = Get-RandWords $word_list $words_per_line
$line3 = Get-RandWords $word_list $words_per_line

$master_string = ($line1 -join " ") + "`n"

Write-Lines @($line1, $line2, $line3)

$StopWatch = New-Object -TypeName System.Diagnostics.Stopwatch 
$StopWatch.start()
$timeout = New-TimeSpan -Seconds 15

while($StopWatch.Elapsed -lt $timeout) {
    $key = $Host.UI.RawUI.ReadKey('NoEcho,IncludeKeyDown')
    $incoming_letter = $key.character

    # if we're at the end of a line
    if ($master_string[$PC] -eq "`n") {
        # dont keep going unless the incoming letter is a space,
        # in which case, jump to the next line
        if ($incoming_letter -ne " ") {
            Write-Char ($PC - 1) $Y $incoming_letter $master_string
            $num_wrong_words++
        } else {
            # logic to jump to the new line
            $PC = 0
            if ($Y -eq 0) {
                $Y++
            } else {
                clear-host
                $line1 = $line2
                $line2 = $line3
                $line3 = Get-RandWords $word_list $words_per_line
                Write-Lines @($line1, $line2, $line3)
            }
            $master_string = ($line2 -join " ") + "`n"
            $num_right_words++
        }
    # if we're at a space
    } elseif ($master_string[$PC] -eq " ") {
        # don't keep going unless the incoming letter is a space
        if ($incoming_letter -ne " ") {
            Write-Char ($PC - 1) $Y $incoming_letter $master_string
            $num_wrong_words++
        } else {
        # keep going because we got a space
            $num_right_words++
            $PC++
        }

    # if we're at a letter
    } elseif (($master_string[$PC] -ge 10) -or ($master_string[$PC] -le 122)) {
        # if the incoming letter is a space, we want to jump to the next word
        if ($incoming_letter -eq " ") {
            # logic to jump to the next word
            while (1) {
                if ($master_string[$PC] -eq " ") {
                    $PC++
                    break
                } elseif ($master_string[$PC] -eq "`n") {
                    $PC = 0
                    if ($Y -eq 0) {
                        $Y++
                    } else {
                        clear-host
                        $line1 = $line2
                        $line2 = $line3
                        $line3 = Get-RandWords $word_list $words_per_line
                        Write-Lines @($line1, $line2, $line3)
                    }
                    $master_string = ($line2 -join " ") + "`n"
                    break
                } else {
                    # if the letter is wrong, write that shit in red
                    Write-Char $PC $Y $master_string[$PC] $master_string "Red"
                    $PC++
                }
            }
            $num_wrong_words++
        
        # if the incoming letter is a letter, just print it out
        } else {
            Write-Char $PC $Y $incoming_letter $master_string
            $PC++
        }
    }
}

write-host "`n`n"

$wpm = $num_right_words / $timeout.TotalMinutes
"your words per minute is:  {0}" -f $wpm

<# 
    TODO:
    - make the cursor be at the right spot
    - support back spaces
    - refactor the code that does the line swaps/ the line breaks
    - keep the colors the same when you go up a line
    - add feature to only start recording when the user starts typing (and add some feedback to indicate that)
    - have the user press a key at the end to quit
    - have the user press 'enter' at the end for more details on their performance
    - add some cools graphs and analytics at the end 
#>