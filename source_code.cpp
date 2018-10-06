// Quick Review Lookup
// Search through thorough thousands of movie reviews from Netflix
// Repository: https://github.com/ramimas/QuickReviewLookup

// Start of header
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <chrono>
using namespace std;
// End of header

class Movie
{
private:
  int M_movieid;
  string M_moviename;
  int M_pubyear;

public:
  // Initialziation of parameter constructor.
  Movie(int m_movieid, string m_moviename, int m_pubyear)
    : M_movieid(m_movieid), M_moviename(m_moviename), M_pubyear(m_pubyear)
  {}

  int getMovieid() const
  {
    return M_movieid;
  }

  auto getMoviename(vector<Movie>& moviesVec, int movieid)
  {
    string name;
    
    for(auto m : moviesVec)
    {
      if(movieid == m.M_movieid)
      {
        name = m.M_moviename;
      }
    }
    
    return name;
  }

  auto getMovieYear(vector<Movie>& moviesVec, int movieid)
  {
    int year;
    
    for(auto m : moviesVec)
    {
      if(movieid == m.M_movieid)
      {
        year = m.M_pubyear;
      }
    }
    
    return year;
  }
};

class Review
{
private:
  int R_reviewid;
  int R_movieid;
  int R_userid;
  int R_rating;
  string R_reviewdate;

public:
  // Initialziation of parameter constructor.
  Review(int r_reviewid, int r_movieid, int r_userid, int r_rating, string r_reviewdate)
    : R_reviewid(r_reviewid), R_movieid(r_movieid), R_userid(r_userid), R_rating(r_rating), R_reviewdate(r_reviewdate)
    {}

  int getReviewid() const
  {
    return R_reviewid;
  }

  auto getAvg(vector<Review>& reviewsVec, int movieid)
  {
    double avg = 0;
    double currentScore = 0;
    double numberReviews = 0;
    int star1 = 0;
    int star2 = 0;
    int star3 = 0;
    int star4 = 0;
    int star5 = 0;

    for(auto r : reviewsVec)
    {
      if(movieid == r.R_movieid)
      {
        currentScore = r.R_rating + currentScore; // If a match was found, add current rating to the current score
        numberReviews++;
        if(r.R_rating == 1){star1++;}
        if(r.R_rating == 2){star2++;}
        if(r.R_rating == 3){star3++;}
        if(r.R_rating == 4){star4++;}
        if(r.R_rating == 5){star5++;}
      }
    }
    if(numberReviews == 0) {avg = 0;}
    else{ avg = currentScore / numberReviews;}

    cout << "Avg rating:  " << avg << endl;
    cout << "Num reviews: " << numberReviews << endl;
    cout << " 1 star:     " << star1 << endl;
    cout << " 2 star:     " << star2 << endl;
    cout << " 3 star:     " << star3 << endl;
    cout << " 4 star:     " << star4 << endl;
    cout << " 5 star:     " << star5 << endl;
  }
  int findMovieid(vector<Review>& reviewsVec, int userInput)
  {
    for(auto r : reviewsVec)
    {
      if(userInput == r.R_reviewid)
      return r.R_movieid;
    }
    return 0;
  }
  auto getMoreInfo(vector<Review>& reviewsVec, int userInput)
  {
    for(auto r : reviewsVec)
    {
      if(userInput == r.R_reviewid)
      {
        cout << "Num stars: " << r.R_rating << endl;
        cout << "User id:   " << r.R_userid << endl;
        cout << "Date:      " << r.R_reviewdate << endl;
      }
    }
  }
  auto getReviewData(vector<Review>& reviewsVec, vector<double>& masterReviewVec)
  {
    vector<double> reviewMovieCounts;  // Vector to store how many times a movie has been reviewed
    vector<double> reviewSumTotal;     // Vector to store the sum of all review scores
    vector<double> reviewAverages;     // Vector to store the average score of each movie
    vector<double> reviewMovieOrder;   // Vector to list all of the movie reviews based on movie id
    vector<double> reviewMovieList;    // Vector to list the reviewed movies
    vector<double> reviewStars;        // Vector to catagorize reviews by number of stars given

    // START STEP 1
    // Loop for all of the the movie ids listed in each review
    for(auto r : reviewsVec)
    {
      // Store each movie ids into the specified vector
      reviewMovieOrder.push_back(r.R_movieid);
    }

    // Sort the vector of newly added movie ids: decending to acending order
    sort(reviewMovieOrder.begin(), reviewMovieOrder.end());
    // END STEP 1

    // START STEP 2
    // Variable declerations 
    int multiplier = 1; // Will count how many times a movie has been reviewed (1 for initial state)
    int newId = 0;      // This will count each new id once;
    int prev = 0;       // This variable is a follower (-1 only for the first step)
    int next = 0;       // This variable is a leader 

    for(auto& r : reviewMovieOrder)
    {
      next = r;

      if(&r == &reviewMovieOrder.back()) // Check if the next value is the last value in the vector
      {
        multiplier++;
        newId = prev; // Store the value of the current id
        reviewMovieCounts.push_back(multiplier); // Push back the amount of times a specific movie id has been counted
        reviewMovieList.push_back(newId);        // Push back the current movie id
        break;
      }

      if(next == prev) // If next and prev are the same movie id
      {
        multiplier++; // Count how many times EACH movie has been reviewed

      }

      else // If next and prev are different, prepare counting for the next movie id.
      {
        // Before we can start counting again we need to save the current information
        newId = prev; // Store the value of the current id
        reviewMovieCounts.push_back(multiplier); // Push back the amount of times a specific movie id has been counted
        reviewMovieList.push_back(newId);        // Push back the current movie id
        multiplier = 1;                          // Reset the multiplier for next loop
      }
      prev = r; // prev is only is set after being compared
    }
    // END STEP 2

    // START STEP 3
    // GET THE SCORE SUM
    int currentScore = 0; // Will hold the sum of all ratings for a specific movie id.
    
    for(auto r : reviewMovieList) // Loop for each movie mentioned in at least one review.
    {
      for(auto n : reviewsVec) // Loop for every review inside of the reviews vector
      {
        if(r == n.R_movieid) // If the movie id's are match, it means there is a review for that movie
        {
          currentScore = n.R_rating + currentScore; // If a match was found, add current rating to the current score
        }
      }

    reviewSumTotal.push_back(currentScore); // Before moving on push back the sum of ratings for each specific movie id
    currentScore = 0; // Reset the current score
    }
    // END STEP 3

    // START STEP 4
    // GET THE AVERAGES
    double average = 0; // Set to double for decimal calculation of average
    int it = 0;
    int it2 = 5;
    vector<double> allinfo;
    for(auto x : reviewSumTotal) // For each loop on the vector that holds all the rating sums
    {
      average = x / reviewMovieCounts[it];              // avg = sum of movie ratings / # of times it's mentioned
      reviewAverages.push_back(average);                // save the averages in its own seperat vector
      allinfo.push_back(average);                       // save the averages 
      allinfo.push_back(reviewMovieList[it]);           // save the specific movie id
      allinfo.push_back(reviewMovieCounts[it]);         // save the number of reviews
      it++;
      average = 0;  // reset the average 
    }

    sort(reviewAverages.begin(), reviewAverages.end(), greater<double>()); // Sort the averages from greatest to least
    // END STEP 4

    it = 0; // reset iterator

    for(auto r : reviewAverages)
    {
      for(auto a : allinfo)
      {
        if(a == r)
         {
           masterReviewVec.push_back(allinfo[it + 1]);    // movie id 
           masterReviewVec.push_back(allinfo[it + 2]);    // number of reviews 
           masterReviewVec.push_back(a);                  // movie avereage
        }
        it++;
      }
      it = 0;
    }
  }
};

// User defined function 1
auto extractMovie(string& moviesFname, vector<Movie>& moviesVec)
{
  ifstream moviesFile(moviesFname); // Open/Close the inputted file

  // Check if the file is valid before moving on
  if (!moviesFile.good())
  {
    cout << "Cannot open movies file!" << endl;
    return -1; // Exit program
  }

  // Declare variables for information in Movies CSV
  string line, m_movieid, m_moviename, m_pubyear;

  getline(moviesFile, line); // Skip the header at the top of the file

  // Time to parse the file
  while(getline(moviesFile, line))
  {
    stringstream ss(line);

    // Seperate data by ','
    getline(ss, m_movieid, ',');
    getline(ss, m_moviename, ',');
    getline(ss, m_pubyear, ',');

    // Store the information into the vector of type Movie
    Movie M(stoi(m_movieid), m_moviename, stoi(m_pubyear));
    moviesVec.push_back(M);
  }
  
  return 0;
}

// User defined function 2
auto extractReview(string& reviewsFname, vector<Review>& reviewsVec)
{
  ifstream reviewsFile(reviewsFname); // Open/Close the inputted file

  // Check if the file is valid before moving on
  if (!reviewsFile.good())
  {
    cout << "Cannot open reviews file!" << endl;
    return -1; // Exit program
  }

  // Declare variables for information in Reviews CSV
  string line, r_reviewid, r_movieid, r_userid, r_reviewdate,r_rating;

  getline(reviewsFile, line); // Skip header at the top of the file.

  // Time to parse the file
  while(getline(reviewsFile, line))
  {
    stringstream ss(line);

    //seperate data by ','
    getline(ss, r_reviewid, ',');
    getline(ss, r_movieid, ',');
    getline(ss, r_userid, ',');
    getline(ss, r_rating, ',');
    getline(ss, r_reviewdate, ',');
    // Store the information into the vector of type Movie
    Review R(stod(r_reviewid), stod(r_movieid), stod(r_userid), stod(r_rating), r_reviewdate);
    reviewsVec.push_back(R);    

  }
  
  return 0;
}
// User defined function 3
map<int, Movie> vec2movieidMap(const vector<Movie>& moviesVec)
{

  map<int, Movie> moviesMap;
  for(const Movie m : moviesVec)
  {
    auto transfer = pair<int, Movie> (m.getMovieid(), m);
    moviesMap.insert(transfer);
  }

  return moviesMap;
}

// User defined function 4
map<int, Review> vec2reviewidMap(const vector<Review>& reviewsVec)
{
  map<int, Review> reviewsMap;
  for(const Review r : reviewsVec)
  {
    auto transfer = pair<int, Review> (r.getReviewid(), r);
    reviewsMap.insert(transfer);
  }

  return reviewsMap;
}

// user defined function 5
void organizeData(vector<Movie>& moviesVec, vector<Review>& reviewsVec, vector<double>& masterReviewVec)
{
  int count = 1; // Rank counter decending to acending 
  int i = 0;
  cout << endl << ">> Top-10 Movies <<" << endl << endl;
  cout << "Rank\tID\tReviews\tAvg\tName" << endl;

  reviewsVec[0].getReviewData(reviewsVec, masterReviewVec);

  // Sort top 10
  while(count <= 10)
  {
    cout << count << "\t" << masterReviewVec[i] << "\t" << masterReviewVec[i+1] << "\t" << masterReviewVec[i+2] << "\t'" <<moviesVec[0].getMoviename(moviesVec, masterReviewVec[i]) << "'" << endl;

    i = i + 3;
    count++;
  }
}

int main()
{
  cout << "** Netflix Movie Review Analysis **" << endl;
  cout << endl;

  // Initialiaze vector containers for sorting the top 10
  vector<double> masterReviewVec; // This is the master vector
                                  // Contains ORGANIZED info 
                                  // Will be loaded up into a MAP for searching

  // Declare variables to store file input
  string moviesFname, reviewsFname; 

  // Extract movie file contents & store them into a vector of type Movie
  cin >> moviesFname; // User will input movies file first
  vector<Movie> moviesVec; // initialize vector
  extractMovie(moviesFname, moviesVec);

  // vec2movieidMap: takes the input vector of movies objects and builds
  // a map of movies ordered by movieid, and returns the map.
  auto moviesMap = vec2movieidMap(moviesVec);

  // Extract review file contents & store them into a vector of type Review
  cin >> reviewsFname; // User will input reviews file next
  vector<Review> reviewsVec; // initialize vector
  extractReview(reviewsFname, reviewsVec);

  //vec2reviewidMap: takes the input vector of review objects and builds
  //a map of movies ordered by movieid, and returns the map.
  auto reviewsMap = vec2reviewidMap(reviewsVec);

  organizeData(moviesVec, reviewsVec, masterReviewVec);

  int userInput = 0;

  cout << endl << ">> Movie and Review Information <<" << endl;

  cout << endl << "Please enter a movie ID (< 100,000), a review ID (>= 100,000), 0 to stop>";

  cin >> userInput;

  while(userInput != 0) //Exit program if user enters zero.
  {
    if(userInput < 0)
    {
      cout << endl << "**invalid id..." << endl;
      cout << endl << "Please enter a movie ID (< 100,000), a review ID (>= 100,000), 0 to stop>";
      cin >> userInput;
      continue;
    }

    if(userInput < 100000)
    {
      auto searchMovieMap = moviesMap.find(userInput);
      if(searchMovieMap != moviesMap.end())
      {
        cout << endl << "Movie:\t     '" << moviesVec[0].getMoviename(moviesVec, userInput) << "'" << endl;
        cout << "Year:\t     " <<  moviesVec[0].getMovieYear(moviesVec, userInput) << endl;
        reviewsVec[0].getAvg(reviewsVec, userInput);
        cout << endl;
      }
      else{
        cout << endl << "movie not found..." << endl;
      }
    }

    if(userInput >= 100000)
    {
      auto searchReviewMap = reviewsMap.find(userInput);
      if(searchReviewMap != reviewsMap.end())
      {
        int movieId = reviewsVec[0].findMovieid(reviewsVec, userInput);
        cout << endl << "Movie: " << movieId << " (" << moviesVec[0].getMoviename(moviesVec, movieId) << ") " << endl;
        reviewsVec[0].getMoreInfo(reviewsVec, userInput);
      }
      else{
        cout << endl << "review not found..." << endl;
      }
    }

    cout << endl << "Please enter a movie ID (< 100,000), a review ID (>= 100,000), 0 to stop>";
    cin >> userInput;

  }
  cout << "** DONE! **" << endl << endl;
  return 0;
}