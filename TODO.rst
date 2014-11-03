Priority 1
----------
* Fix this bug: on online tests (I don't know about offline), when you add a theme which the database assigns an ID that is not equal to (the last ID+1), and you go back to the test to select the new theme, it crashes.
* Implement tag imports during list imports:
  Before importing words, make a list of the tags used in the list to import, find which are the new ones (according to their names), add them to the local tags, and then import the words and insert the word-tag relations *according to the local tag ids*.
  The tag name comparisions should normalize them: trim spaces, ignore case, maybe ignore some other things too.

Priority 2
----------
* Try to adapt the app for a mobile version

Priority 3
----------
* Don't care for spaces in correct pronunciation of an asked word (standardize it in a form that doesn't contain spaces)
* Button to delete a word
* Move word-specific buttons inside question/answer views
* Find a way to map language ISO codes to their names
* Create app icon: My idea is to represent a nice chocolate cake with crème anglaise spilling on it, like this: 
  http://www.howdoyousaythatword.com/wp-content/uploads/2012/01/creme-anglaise.jpg, use a vector graphics editor.
* Treat all cases when there is no Internet connection.
* Change font sizes, especially the word being asked
* Set a default window width (?)
* Improve ergonomics: allow to do everything using the keyboard only.

  * An "OK" button should always be pressable with the Enter key or Ctrl+Enter if there is an ambiguity.
  * A "Cancel" or "Go back" button should always be pressable with the Esc key.
  * All the modifiable widgets should be accessible with Tab.
  
* Improve search:

  * Develop a search string as multiple search strings with different functions applied to them: for example in Chinese, develop a search "ma3" into a search for "mǎ", develop a search "ma" into searches for "mā", "má", "mǎ", "mà".
  * Allow searches for multiple strings at a time
  * Highlight matched strings
  
* Publicly release the app with an installer
* Include a field for the gender in German
* Being able to import an online vocabulary list, or a sublist, or a single word. In each case, a new view will appear asking the user if they want to make it a new list, or import it into an existing list. In the latter case, check if the `word` field of the inserted items don't already exist in the existing list. Let the user choose to:

  * only keep the already existing item if there is a conflict
  * write over existing items
  * merge them, that is, for a ``meaning`` field "x" offline, and "y" online, set it to "x, y"
  * ask for every item where there is a conflict
  * (edit manually?)
  
* Add a combo box to filter by theme in the search view
