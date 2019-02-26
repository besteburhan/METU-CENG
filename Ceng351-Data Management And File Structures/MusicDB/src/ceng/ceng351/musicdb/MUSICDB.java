package ceng.ceng351.musicdb;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class MUSICDB implements IMUSICDB {
      private static String user = "e2171395";
      private static String password = "be4bc56c";
      private static String host = "144.122.71.57";
      private static String database = "db2171395";
      private static int port = 8084;

      private Connection con;

  public MUSICDB(){

  }
    /**
    * Place your initialization code inside if required.
    *
    * <p>
    * This function will be called before all other operations. If your implementation
    * need initialization , necessary operations should be done inside this function. For
    * example, you can set your connection to the database server inside this function.
    */
   public void initialize(){
          String url = "jdbc:mysql://" + this.host + ":" + this.port + "/" + this.database;

           try {
               Class.forName("com.mysql.jdbc.Driver");
               this.con =  DriverManager.getConnection(url, this.user, this.password);
           } catch (SQLException | ClassNotFoundException e) {
               e.printStackTrace();
           }
         }
   /**
    * Should create the necessary tables when called.
    *
    * @return the number of tables that are created successfully.
    */
   public int createTables(){
     int result;
     int numberofTablesInserted = 0;

     String queryCreateUserTable = "create table if not exists User("+
                                                          "userID int not null ," +
                                                          "userName varchar(60) ," +
                                                          "email varchar(30)," +
                                                          "password varchar(30)," +
                                                          "primary key (userID))";

    try {
        Statement statement = this.con.createStatement();

        result = statement.executeUpdate(queryCreateUserTable);
        numberofTablesInserted++;
        //close
        statement.close();
    } catch (SQLException e) {
        e.printStackTrace();
        }

    String queryCreateArtistTable = "create table if not exists Artist("+
                                                               "artistID int not null ," +
                                                               "artistName varchar(60) ,"+
                                                               "primary key (artistID))" ;

         try {
             Statement statement = this.con.createStatement();

             result = statement.executeUpdate(queryCreateArtistTable);
             numberofTablesInserted++;
             //close
             statement.close();
         } catch (SQLException e) {
             e.printStackTrace();
             }

    String queryCreateAlbumTable = "create table if not exists Album("+
                                                                    "albumID int not null," +
                                                                    "title varchar(60) ," +
                                                                    "albumGenre varchar(30)," +
                                                                    "albumRating double," +
                                                                    "releaseDate date," +
                                                                    "artistID int," +
                                                                    "primary key (albumID)," +
                                                                    "foreign key (artistID) references Artist(artistID) on delete cascade on update cascade)";

         try {
                  Statement statement = this.con.createStatement();

                  result = statement.executeUpdate(queryCreateAlbumTable);
                  numberofTablesInserted++;
                  //close
                  statement.close();
         } catch (SQLException e) {
                  e.printStackTrace();
                  }


    String queryCreateSongTable = "create table if not exists Song("+
                                                             "songID int not null," +
                                                             "songName varchar(60) ," +
                                                             "genre varchar(30)," +
                                                             "rating double," +
                                                             "artistID int," +
                                                             "albumID int," +
                                                             "primary key (songID)," +
                                                             "foreign key (artistID) references Artist(artistID) on delete cascade on update cascade,"+
                                                             "foreign key (albumID) references Album(albumID)on delete cascade on update cascade)";

    try {
           Statement statement = this.con.createStatement();

           result = statement.executeUpdate(queryCreateSongTable);
           numberofTablesInserted++;
           //close
           statement.close();
       } catch (SQLException e) {
           e.printStackTrace();
           }


      String queryCreateListenTable = "create table if not exists Listen("+
                                                                  "userID int not null ," +
                                                                  "songID int not null," +
                                                                  "lastListenTime timestamp," +
                                                                  "listenCount int," +
                                                                  "primary key (userID,songID)," +
                                                                  "foreign key (userID) references User(userID) on delete cascade on update cascade,"+
                                                                  "foreign key (songID) references Song(songID) on delete cascade on update cascade)";

            try {
                Statement statement = this.con.createStatement();

                result = statement.executeUpdate(queryCreateListenTable);
                numberofTablesInserted++;
                //close
                statement.close();
            } catch (SQLException e) {
                e.printStackTrace();
                }


     return numberofTablesInserted;
   }

   /**
    * Should drop the tables if exists when called.
    *
    * @return the number of tables are dropped successfully.
    */
   public int dropTables(){
        int result;
        int numberofTablesDropped = 0;

        String queryDropListenTable = "drop table if exists Listen";

        try {
          Statement statement = this.con.createStatement();
          result = statement.executeUpdate(queryDropListenTable);
          numberofTablesDropped++;
           //close
          statement.close();

          } catch (SQLException e) {
                 e.printStackTrace();
          }
        String queryDropSongTable = "drop table if exists Song";

        try {
            Statement statement = this.con.createStatement();
            result = statement.executeUpdate(queryDropSongTable);
            numberofTablesDropped++;
             //close
            statement.close();

            } catch (SQLException e) {
                   e.printStackTrace();
            }
        String queryDropAlbumTable = "drop table if exists Album";

        try {
              Statement statement = this.con.createStatement();
              result = statement.executeUpdate(queryDropAlbumTable);
              numberofTablesDropped++;
               //close
              statement.close();

              } catch (SQLException e) {
                     e.printStackTrace();
              }
        String queryDropArtistTable = "drop table if exists Artist";

              try {
                Statement statement = this.con.createStatement();
                result = statement.executeUpdate(queryDropArtistTable);
                numberofTablesDropped++;
                 //close
                statement.close();

                } catch (SQLException e) {
                       e.printStackTrace();
                }
        String queryDropUserTable = "drop table if exists User";

              try {
                  Statement statement = this.con.createStatement();
                  result = statement.executeUpdate(queryDropUserTable);
                  numberofTablesDropped++;
                   //close
                  statement.close();

                  } catch (SQLException e) {
                         e.printStackTrace();
                  }

          return numberofTablesDropped;
   }

   /**
    * Should insert an array of Album into the database.
    *
    * @return Number of rows inserted successfully.
    */
   public int insertAlbum(Album[] albums){
    int result = 0;
    int numberOfRows=0;

      for(Album album : albums){
        String query = "insert into Album values (\"" + 
                        album.getAlbumID()+ "\",\"" + 
                        album.getTitle() + "\",\"" + 
                        album.getAlbumGenre() + "\",\"" + 
                        album.getAlbumRating() + "\",\"" + 
                        album.getReleaseDate() + "\",\"" + 
                        album.getArtistID() + "\")";

        try {
            Statement st = this.con.createStatement();
            result = st.executeUpdate(query);
            numberOfRows++;
            //Close
            st.close();

        } catch (SQLException e) {
            e.printStackTrace();
            //if (e.toString().contains("SQLIntegrityConstraintViolationException")){
             //   throw new AlreadyInsertedException();
            //}
        }
      }
     return numberOfRows;
   }

   /**
    * Should insert an array of Artist into the database.
    *
    * @return Number of rows inserted successfully.
    */
   public int insertArtist(Artist[] artists){
      int result = 0;
      int numberOfRows=0;

      for(Artist artist : artists){
        String query = "insert into Artist values (\"" + 
                        artist.getArtistID()+ "\",\"" + 
                        artist.getArtistName() + "\")";

        try {
            Statement st = this.con.createStatement();
            result = st.executeUpdate(query);
            numberOfRows++;
            //Close
            st.close();

        } catch (SQLException e) {
            e.printStackTrace();
            //if (e.toString().contains("SQLIntegrityConstraintViolationException")){
             //   throw new AlreadyInsertedException();
            //}
        }
      }
     return numberOfRows;
   }

   /**
    * Should insert an array of Song into the database.
    *
    * @return Number of rows inserted successfully.
    */
   public int insertSong(Song[] songs){
      int result = 0;
      int numberOfRows=0;

      for(Song song : songs){
        String query = "insert into Song values (\"" + 
                        song.getSongID()+ "\",\"" + 
                        song.getSongName() + "\",\"" + 
                        song.getGenre() + "\",\"" + 
                        song.getRating() + "\",\"" + 
                        song.getArtistID() + "\",\"" + 
                        song.getAlbumID() + "\")";

        try {
            Statement st = this.con.createStatement();
            result = st.executeUpdate(query);
            numberOfRows++;
            //Close
            st.close();

        } catch (SQLException e) {
            e.printStackTrace();
            //if (e.toString().contains("SQLIntegrityConstraintViolationException")){
             //   throw new AlreadyInsertedException();
            //}
        }
      }
     return numberOfRows;
   }

   /**
    * Should insert an array of User into the database.
    *
    * @return Number of rows inserted successfully.
    */
   public int insertUser(User[] users){
      int result = 0;
      int numberOfRows=0;
      for(User user : users){
        String query = "insert into User values (\"" + 
                        user.getUserID()+ "\",\"" + 
                        user.getUserName() + "\",\"" + 
                        user.getEmail() + "\",\"" + 
                        user.getPassword() + "\")";

        try {
            Statement st = this.con.createStatement();
            result = st.executeUpdate(query);
            numberOfRows++;
            //Close
            st.close();

        } catch (SQLException e) {
            e.printStackTrace();
            //if (e.toString().contains("SQLIntegrityConstraintViolationException")){
             //   throw new AlreadyInsertedException();
            //}
        }
      }
     return numberOfRows;
   }

   /**
    * Should insert an array of Listen into the database.
    *
    * @return Number of rows inserted successfully.
    */
   public int insertListen(Listen[] listens){
      int result = 0;
      int numberOfRows=0;

      for(Listen listen : listens){
        String query = "insert into Listen values (\"" + 
                        listen.getUserID()+ "\",\"" + 
                        listen.getSongID() + "\",\"" + 
                        listen.getLastListenTime() + "\",\"" + 
                        listen.getListenCount() + "\")";

        try {
            Statement st = this.con.createStatement();
            result = st.executeUpdate(query);
            numberOfRows++;
            //Close
            st.close();

        } catch (SQLException e) {
            e.printStackTrace();
            //if (e.toString().contains("SQLIntegrityConstraintViolationException")){
             //   throw new AlreadyInsertedException();
            //}
        }
      }
     return numberOfRows;
   }

   /**
    * Should return songs with the highest rating
    *
    * @return ArtistNameSongNameGenreRatingResult[]
    */
   public QueryResult.ArtistNameSongNameGenreRatingResult[] getHighestRatedSongs(){

    ResultSet rs;
    QueryResult.ArtistNameSongNameGenreRatingResult[] res = null;
    String query = "select distinct A.artistName , S1.songName, S1.genre, S1.rating "+
                    "from Song S1, Artist A "+
                    "where S1.artistID=A.artistID and S1.rating >= ( select Max(Song.rating) from Song) order by A.artistName;";
    try {
            Statement st = this.con.createStatement();
            rs = st.executeQuery(query); 
            int rowcount = 0;
            if (rs.last()) {
                rowcount = rs.getRow();
                rs.beforeFirst(); // not rs.first() because the rs.next() below will move on, missing the first element
            }
            res = new QueryResult.ArtistNameSongNameGenreRatingResult[rowcount];
            int i=0;
            while(rs.next()){
                String artistName = rs.getString("artistName");
                String songName = rs.getString("songName");
                String genre = rs.getString("genre");
                double rating = rs.getDouble("rating");
                QueryResult.ArtistNameSongNameGenreRatingResult elem = new QueryResult.ArtistNameSongNameGenreRatingResult(artistName,songName,genre,rating);
                res[i] = elem;
                i++;
            }

            //Close
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

     return res;
   }

   /**
    * Should return the most recent album for the given artistName
    *
    * @return TitleReleaseDateRatingResult
    */
   public QueryResult.TitleReleaseDateRatingResult getMostRecentAlbum(String artistName){
    ResultSet rs;
    QueryResult.TitleReleaseDateRatingResult res = null;
    String query = "select A.title, A.releaseDate ,A.albumRating "+
                    "from Album A, Artist W "+
                    "where W.artistName = '"+artistName+"' and W.artistID = A.artistID and A.releaseDate >= (select max(Album.releaseDate) from Album );";
    try {
            Statement st = this.con.createStatement();
            rs = st.executeQuery(query); 
            rs.next();
            String title = rs.getString("title");
            String releaseDate = rs.getString("releaseDate");
            double rating = rs.getDouble("albumRating");

            res = new QueryResult.TitleReleaseDateRatingResult(title,releaseDate,rating);

            //Close
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

     return res;

   }

   /**
    * Should return songs that are listened by both userName1 & userName2
    *
    * @return ArtistNameSongNameGenreRatingResult[]
    */
   public QueryResult.ArtistNameSongNameGenreRatingResult[] getCommonSongs(String userName1, String userName2){
    ResultSet rs;
    QueryResult.ArtistNameSongNameGenreRatingResult[] res = null;
    String query = "select distinct A.artistName, S.songName, S.genre, S.rating "+
                    "from Listen L1, Listen L2, Song S, Artist A "+
                    "where L1.songID = L2.songID and L1.userID in (select userID from User where userName = \"" + userName1 +
                          "\") and L2.userID in  (select userID from User where userName=\"" + userName2 +
                          "\") and L1.userID <>L2.userID and S.songID = L1.songID and S.artistID = A.artistID order by  S.rating desc;";
    try {
            Statement st = this.con.createStatement();
            rs = st.executeQuery(query); 
            int rowcount = 0;
            if (rs.last()) {
                rowcount = rs.getRow();
                rs.beforeFirst(); // not rs.first() because the rs.next() below will move on, missing the first element
            }
            res = new QueryResult.ArtistNameSongNameGenreRatingResult[rowcount];
            int i=0;
            while(rs.next()){
                String artistName = rs.getString("artistName");
                String songName = rs.getString("songName");
                String genre = rs.getString("genre");
                double rating = rs.getDouble("rating");
                QueryResult.ArtistNameSongNameGenreRatingResult elem = new QueryResult.ArtistNameSongNameGenreRatingResult(artistName,songName,genre,rating);
                res[i] = elem;
                i++;
            }

            //Close
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

     return res;
   }

   /**
    * Should return artists whose songs are listened by the given user
    * and number of times his/her songs have been listened by the given user
    *
    * @return ArtistNameNumberOfSongsResult[]
    */
   public QueryResult.ArtistNameNumberOfSongsResult[] getNumberOfTimesSongsListenedByUser(String userName){
    ResultSet rs;
    QueryResult.ArtistNameNumberOfSongsResult[] res = null;
    String query = "select distinct A.artistName, sum(L.listenCount) as numberOfSongs "+
                    "from Artist A, Listen L, Song S "+
                    "where L.userID in (select userID from User where userName = \"" + userName +
                          "\") and L.songID = S.songID "+
                          "and S.artistID = A.artistID group by A.artistID,A.artistName order by A.artistName;";
    try {
            Statement st = this.con.createStatement();
            rs = st.executeQuery(query); 
            int rowcount = 0;
            if (rs.last()) {
                rowcount = rs.getRow();
                rs.beforeFirst(); // not rs.first() because the rs.next() below will move on, missing the first element
            }
            res = new QueryResult.ArtistNameNumberOfSongsResult[rowcount];
            int i=0;
            while(rs.next()){
                String artistName = rs.getString("artistName");
                int numberOfSongs = rs.getInt("numberOfSongs");
                QueryResult.ArtistNameNumberOfSongsResult elem = new QueryResult.ArtistNameNumberOfSongsResult(artistName,numberOfSongs);
                res[i] = elem;
                i++;
            }

            //Close
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

     return res;

   }

   /**
   * Should return users who have listened all songs of a given artist
    *
    * @return User[]
    */
   public User[] getUsersWhoListenedAllSongs(String artistName){
    ResultSet rs;
    User[] res = null;
    String query = "select * from User U where not exists ( select  S.songID "
                    +"from Artist A , Song S where A.artistName = \""+artistName+
                    "\" and A.artistID = S.artistID and not exists ( "+
                    "select L.songID from Listen L where L.userID = U.userID"+
                    " and L.songID = S.songID) order by U.userID);";
    try {
            Statement st = this.con.createStatement();
            rs = st.executeQuery(query); 
            int rowcount = 0;
            if (rs.last()) {
                rowcount = rs.getRow();
                rs.beforeFirst(); // not rs.first() because the rs.next() below will move on, missing the first element
            }
            res = new User[rowcount];
            int i=0;
            while(rs.next()){
                int userID = rs.getInt("userID");
                String userName = rs.getString("userName");
                String email = rs.getString("email");
                String password = rs.getString("password");
                User elem = new User(userID,userName,email,password);
                res[i] = elem;
                i++;
            }
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

     return res;

   }

   /**
    * Should return the userID, userName and number of songs listened by this user such that
    * none of these songs are listened by any other user.
    *
    * @return QueryResult.UserIdUserNameNumberOfSongsResult[]
    */
   public QueryResult.UserIdUserNameNumberOfSongsResult[]  getUserIDUserNameNumberOfSongsNotListenedByAnyone(){
    ResultSet rs;
    QueryResult.UserIdUserNameNumberOfSongsResult[] res = null;
    String query = "select distinct U.userID, U.userName, cnt.num "+
                   "from (select L.userID, Count(*) as num from  Listen L where L.songID not in ( select L2.songID from  Listen L2 where L2.userID <> L.userID )  group by L.userID) as cnt, User U "+
                   "where U.userID = cnt.userID order by U.userID";
    try {
            Statement st = this.con.createStatement();
            rs = st.executeQuery(query); 
            int rowcount = 0;
            if (rs.last()) {
                rowcount = rs.getRow();
                rs.beforeFirst(); // not rs.first() because the rs.next() below will move on, missing the first element
            }
            res = new QueryResult.UserIdUserNameNumberOfSongsResult[rowcount];
            int i=0;
            while(rs.next()){
                int userID = rs.getInt("userID");
                String userName = rs.getString("userName");
                int num = rs.getInt("num");
                QueryResult.UserIdUserNameNumberOfSongsResult elem = new QueryResult.UserIdUserNameNumberOfSongsResult(userID,userName,num);
                res[i] = elem;
                i++;
            }

            //Close
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

     return res;

   }

   /**
    * Should return artists who have sung pop music at least once and whose average rating of
    * all songs is greater than the given rating
    *
    * @return Artist[]
    */
   public Artist[] getArtistSingingPopGreaterAverageRating(double rating){
    ResultSet rs;
    Artist[] res = null;
    String query = "select distinct A.artistID, A.artistName from Artist A, Song S "+
                    "where A.artistID = S.artistID and "+
                            "exists ( select S1.songID from Song S1 where S1.artistID = A.artistID and S1.genre = \"pop\") "+
                    "group by A.artistID having avg(S.rating) > "  +rating+" order by A.artistID;";
    try {
            Statement st = this.con.createStatement();
            rs = st.executeQuery(query); 
            int rowcount = 0;
            if (rs.last()) {
                rowcount = rs.getRow();
                rs.beforeFirst(); // not rs.first() because the rs.next() below will move on, missing the first element
            }
            res = new Artist[rowcount];
            int i=0;
            while(rs.next()){
                int artistID = rs.getInt("artistID");
                String artistName = rs.getString("artistName");
                Artist elem = new Artist(artistID,artistName);
                res[i] = elem;
                i++;
            }
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

     return res;

   }

   /**
    * Retrieve songs with the lowest rating in pop category, in case of more than one song exist,
    * retrieve the least listened ones
    *
    * @return Song[]
    */
   public Song[] retrieveLowestRatedAndLeastNumberOfListenedSongs(){
    ResultSet rs;
    Song[] res = null;
    String query = "select distinct S1.songID, S1.songName, S1.genre, S1.rating, S1.artistID, S1.albumID "+
                   "from (select distinct  L.songID, sum(L.listenCount) as num from Song S, Listen L where S.genre=\"pop\"  and S.rating = (select min(rating) from Song where genre =\"pop\") and L.songID = S.songID group by L.songID) as temp, Song S1 "+
                   "where temp.num <=all ((select sum(L.listenCount) as num from Song S, Listen L where S.genre=\"pop\"  and S.rating = (select min(rating) from Song where genre =\"pop\") and L.songID = S.songID group by L.songID))and S1.songID = temp.songID order by S1.songID;";
    try {
            Statement st = this.con.createStatement();
            rs = st.executeQuery(query); 
            int rowcount = 0;
            if (rs.last()) {
                rowcount = rs.getRow();
                rs.beforeFirst(); // not rs.first() because the rs.next() below will move on, missing the first element
            }
            res = new Song[rowcount];
            int i=0;
            while(rs.next()){
              int songID = rs.getInt("songID");
              String songName =rs.getString("songName");
              String genre=rs.getString("genre");
              double rating=rs.getDouble("rating");
              int artistID=rs.getInt("artistID");
              int albumID=rs.getInt("albumID");
              Song elem = new Song(songID,songName,genre,rating,artistID,albumID);
              res[i] = elem;
              i++;
            }
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

     return res;
   }
   /**
    * Multiply the rating of the album by 1.5 whose releaseDate is after for a given date
    *
    * @return the row count for SQL Data Manipulation Language (DML) statements
    */
   public int multiplyRatingOfAlbum(String releaseDate){
    int res=0;
    String query = "update Album set albumRating = albumRating*1.5 where releaseDate >\" "+
    releaseDate + "\" ;";
    try {
      Statement st = this.con.createStatement();
      res = st.executeUpdate(query);
      st.close();
    } catch (SQLException e) {
              e.printStackTrace();
    }
     return res;
   }

   /**
    * Delete the song for the given songName
    *
    * @return Song
    */
   public Song deleteSong(String songName){
    ResultSet rs;
    Song res=null;
    int n=0;
    String query1 = "select * from Song where songName=\""+songName+"\";";
    String query = "delete from Song where songName=\""+songName+"\";";
    try {
      Statement st = this.con.createStatement();
      rs = st.executeQuery(query1);
      rs.next();
      int songID = rs.getInt("songID");
      String sName =rs.getString("songName");
      String genre=rs.getString("genre");
      double rating=rs.getDouble("rating");
      int artistID=rs.getInt("artistID");
      int albumID=rs.getInt("albumID");
      res = new Song(songID,sName,genre,rating,artistID,albumID);
      n= st.executeUpdate(query);
      st.close();
    } catch (SQLException e) {
              e.printStackTrace();
    }
     return res;
   }

}
