To do first (because very useful):
==================================

* Enable using a list from an offline database
* Create widget to draw characters for Asian languages
* Marking system: define a mark for each word (representing the level of learning of this word). Set everything to 0 in the beginning. When a word is correct, increment the mark. If it is wrong, decrement it. Always ask words with the lowest marks.

To do next:
===========

* Button ``Add another word`` in widget ``Add a word``.
* App mustn't crash when clicking on ``Add new word`` while the asked word is still loading.
* Treat all cases when there is no Internet connection.
* App mustn't crash when clicking on ``Back to test`` from add frame.
* Ensure that every time an object is created with ``new``, either it has never been defined before, or it has been deleted with ``delete``. Either way there'll be memory leaks.
* Understand why some ``delete`` make the app crash
* Color the edit field in green or red when right or wrong and color those messages too
* Change font sizes, especially the word being asked
* Set a default window width (?)
* Create app icon: My idea is to represent a nice chocolate cake with crème anglaise spilling on it, like this: http://www.howdoyousaythatword.com/wp-content/uploads/2012/01/creme-anglaise.jpg, use a vector graphics editor.
* Group words by theme
