# Naive-Bayes-text-categorizer
This program accepts training and testing data to train and test a Naive Bayes classifier for text categorization.
The program is written in C++, and was designed as part of the ECE467 Natural Language Processing course at the Cooper Union.
Unigrams (words) are used as the individual features for the Naive Bayes classifier.
Uppercase letters are converted to lowercase, and all punctuation, save for the hyphen, is discarded.
To facilitate the classifier, document counts (rather than word counts) are used to estimate unigram probabilities.
Laplace smoothing is used to account for novel words in the training set.
Log probabilities are used to facilitate computation.
