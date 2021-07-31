<###################################################################################
                            COMMAND LINE ARGUMENTS
###################################################################################>
param (
    [int]$time = 200,
    [int]$wordsPerLine = 5
)

<###################################################################################
                                   CONSTANTS
###################################################################################>
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

<###################################################################################
                                    FUNCTIONS
###################################################################################>
    
function Get-RandWords ($list, $num) {
    $rand_words = @()
    for($i = 0; $i -lt $num; $i++) {
        $rand_words += (get-random -InputObject $list)
    }
    return $rand_words
}
function Write-Lines ($lines, $offset) {
    if ($offset) {
        $Y = $offset
    } else {
        $Y = 0
    }
    foreach ($line in $lines) {
        $X = 0
        $width = $Host.UI.RawUI.WindowSize.Width  
        $offset = 0
        $temp = $line[$lines[2].Length + 5]
        if ($null -ne $temp) {
            [int]$offset = ($width / 2) - (($line.Length + 1) / 2)
        }
        else {
            [int]$offset = ($width / 2) - ((($line -join " ").Length + 1) / 2)
        }
        # write-host ("`n`n`n`n`n{0}" -f $offset)
        foreach ($word in $line) {
            if ($word -is [string]) {
                $host.UI.RawUI.CursorPosition = @{ x = $X + $offset; y = $Y }
                write-host $word -NoNewline
                write-host " " -NoNewline
                $X += $word.Length + 1
            } else {
                $host.UI.RawUI.CursorPosition = @{ x = $X + $offset; y = $Y }
                write-host $word[0] -NoNewline -ForegroundColor $word[1]
                $X++
            }
        }
        $Y++
    }
}

function Write-Char ($x, $y, $letter, $master_string, $color) {
    $width = $Host.UI.RawUI.WindowSize.Width
    [int]$offset = ($width / 2) - ($master_string.Length / 2)
    $host.UI.RawUI.CursorPosition = @{ x = $x + $offset; y = $y }  
    if ($color) {
        Write-Host $letter -ForegroundColor $color -NoNewline
    } elseif ($letter -eq $master_string[$PC]) {
        Write-Host $letter -ForegroundColor Yellow -NoNewline
    } else {
        Write-Host $letter -ForegroundColor Red -NoNewline
    }
}

# non blocking get keypress function
function Get-KeyPress {
    if ([Console]::KeyAvailable) {
        return [Console]::ReadKey($true)
    }
}

function Get-CursorPosition ($x, $y) {

}

function Show-Timer ($stopwatch, $last_second_value, $y_coord, $total_time) {
    #print the timer
    $width = $Host.UI.RawUI.WindowSize.Width
    $time_s = ([timespan]::fromseconds($total_time.TotalSeconds) - 
               [timespan]::fromseconds($StopWatch.Elapsed.TotalSeconds))
    $time_str = ("{0:mm\:ss}" -f $time_s)
    [int]$offset = ($width / 2) - ($time_str.Length  / 2)
    $last_cursor_pos = $host.UI.RawUI.CursorPosition
    $host.UI.RawUI.CursorPosition = @{ x = $offset; y = $y_coord }
    write-host $time_str -NoNewLine
    $host.UI.RawUI.CursorPosition = @{ x = $last_cursor_pos.X; y = $last_cursor_pos.Y }
}
<###################################################################################
                                  APPLICATION CODE
###################################################################################>


# declare some global variables
$OFFSET_Y = 2
$OFFSET_Y_TIMER = 0
$WORDS_PER_LINE = $wordsPerLine
$PC = 0
$Y = 0
$last_second_value = 100
$num_right_words = 0
$num_wrong_words = 0
$recorded_colors = @()
$line1 = Get-RandWords $word_list $WORDS_PER_LINE
$line2 = Get-RandWords $word_list $WORDS_PER_LINE
$line3 = Get-RandWords $word_list $WORDS_PER_LINE
$master_string = ($line1 -join " ") + "`n"


$width = $Host.UI.RawUI.WindowSize.Width

# print the opening prompt
Clear-Host
$info_msg = "START TYPING TO START THE TEST..."
[int]$offset = ($width / 2) - ($info_msg.Length / 2)
$host.UI.RawUI.CursorPosition = @{ x = $offset; y = $Y }
write-host $info_msg
[int]$offset = ($width / 2) - ($master_string.Length / 2)
$host.UI.RawUI.CursorPosition = @{ x = $offset; y = $Y }  
Write-Lines @($line1, $line2, $line3) -offset $OFFSET_Y
$host.UI.RawUI.CursorPosition = @{ x = $offset; y = ($Y + $OFFSET_Y) }  

# wait for any key to be pressed and star the test
# $key = $Host.UI.RawUI.ReadKey('NoEcho,IncludeKeyDown')
$first_keypress = $null
while(1) {
    $first_keypress = Get-KeyPress
    if ($null -ne $first_keypress) {break}
}

# now print the opening lines and start the test
Clear-Host
Write-Lines @($line1, $line2, $line3) -offset $OFFSET_Y 

$StopWatch = New-Object -TypeName System.Diagnostics.Stopwatch 
$StopWatch.start()
$timeout = New-TimeSpan -Seconds $time

while($StopWatch.Elapsed -lt $timeout) {
    $key = $null
    if ($null -ne $first_keypress) {
        $key = $first_keypress
        $first_keypress = $null
    } else {
        $key = Get-KeyPress
    }
    if ($key) {
        [string]$incoming_letter = $key.key

        $incoming_letter = $incoming_letter.ToLower()

        if ($incoming_letter -eq "spacebar") {
            $incoming_letter = " "
        }
        # if we get an enter, do nothing
        if ($incoming_letter -eq "enter") {
            # pass
        }
    
        # if we get a backspace, do nothing
        elseif ($incoming_letter -eq "backspace") {
            # pass
        }
    
        # if we're at a newline
        elseif ($master_string[$PC] -eq "`n") {
            # dont keep going unless the incoming letter is a space,
            # in which case, jump to the next line
            if ($incoming_letter -ne " ") {
                Write-Char ($PC - 1) ($Y + $OFFSET_Y) $incoming_letter $master_string
                $recorded_colors[($PC - 1)] = @($incoming_letter, "Red")
                # TODO: should only increment wrong words the first time. this will keep recording wrong words even though you only got one word wrong
                $num_wrong_words++
            } else {
                # logic to jump to the new line
                $PC = 0
                if ($Y -eq 0) {
                    $Y++
                } else {
                    clear-host
                    Show-Timer -stopwatch $StopWatch -y_coord $OFFSET_Y_TIMER -total_time $timeout
                    $line1 = $recorded_colors
                    $line2 = $line3
                    $line3 = Get-RandWords $word_list $WORDS_PER_LINE
                    Write-Lines @($line1, $line2, $line3) -offset $OFFSET_Y
                }
                $recorded_colors = @()
                $master_string = ($line2 -join " ") + "`n"
                $num_right_words++
                $width = $Host.UI.RawUI.WindowSize.Width
                [int]$offset = ($width / 2) - ($master_string.Length  / 2)
                $host.UI.RawUI.CursorPosition = @{ x = $offset; y = ($Y + $OFFSET_Y)}
            }
        }
    
        # if we're at a space
        elseif ($master_string[$PC] -eq " ") {
            # don't keep going unless the incoming letter is a space
            if ($incoming_letter -ne " ") {
                Write-Char ($PC - 1) ($Y + $OFFSET_Y) $incoming_letter $master_string
                $recorded_colors[($PC - 1)] = @($incoming_letter, "Red")
                $num_wrong_words++
            } else {
                # keep going because we got a space
                Write-Char $PC ($Y + $OFFSET_Y) $incoming_letter $master_string
                $num_right_words++
                $PC++
                $recorded_colors += ,@(" ", "Yellow")
            }
        }
    
        # if we're at a letter
        elseif (($master_string[$PC] -ge 10) -or ($master_string[$PC] -le 122)) {
            # if the incoming letter is a space, we want to jump to the next word
            if ($incoming_letter -eq " ") {
                # logic to jump to the next word
                while (1) {
                    if ($master_string[$PC] -eq " ") {
                        $PC++
                        $recorded_colors += ,@(" ", "Yellow")
                        break
                    } elseif ($master_string[$PC] -eq "`n") {
                        $PC = 0
                        if ($Y -eq 0) {
                            $Y++
                        } else {
                            clear-host
                            Show-Timer -stopwatch $StopWatch -y_coord $OFFSET_Y_TIMER -total_time $timeout
                            $line1 = $recorded_colors
                            $line2 = $line3
                            $line3 = Get-RandWords $word_list $WORDS_PER_LINE
                            Write-Lines @($line1, $line2, $line3) -offset $OFFSET_Y
                        }
                        $recorded_colors = @()
                        $master_string = ($line2 -join " ") + "`n"
                        $width = $Host.UI.RawUI.WindowSize.Width
                        [int]$offset = ($width / 2) - ($master_string.Length  / 2)
                        $host.UI.RawUI.CursorPosition = @{ x = $offset; y = ($Y + $OFFSET_Y) }
                        break
                    } else {
                        # if the letter is wrong, write that shit in red
                        Write-Char $PC ($Y + $OFFSET_Y) $master_string[$PC] $master_string "Red"
                        $recorded_colors += ,@($master_string[$PC], "Red")
                        $PC++
                    }
                }
                $num_wrong_words++
            
            # if the incoming letter is a letter, just print it out
            } else {
                if ($incoming_letter -eq $master_string[$PC]) {
                    $recorded_colors += ,@($incoming_letter, "Yellow")
                } else {
                    $recorded_colors += ,@($incoming_letter, "Red")
                }
                Write-Char $PC ($Y + $OFFSET_Y) $incoming_letter $master_string
                $PC++
            }
        }
    }

    # display the timer
    $time = [int]$Stopwatch.Elapsed.TotalSeconds
    if ($saved_time -ne $time) {
        Show-Timer -stopwatch $StopWatch -y_coord $OFFSET_Y_TIMER -total_time $timeout
    }
    $saved_time = $time
}

write-host "`n`n"

$wpm = $num_right_words / $timeout.TotalMinutes
"your words per minute is:  {0}" -f $wpm

<# 
    TODO:
    - show a timer showing how much time remaining
    - have the user press 'enter' at the end for more details on their performance
    - refactor out the code that gets the position of the cursor

    - maybe get rid of the cursor, but use a different color text for the position
#>

<#
    Longer term goals:
    - support back spaces
    - refactor / cleanup the while loop (?)
    - use function parameters for all function calls
#>

<#
    BUGS:
    - TODO on line 132
#>

# debug logger
# Add-Content debug.log ("clause 1 -- Width: {0} Line Length: {1}" -f $width, $line.Length)
