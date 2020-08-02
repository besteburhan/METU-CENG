import java.util.List;
import java.util.function.IntBinaryOperator;
import java.util.function.Predicate;
import java.util.stream.Collector;
import java.util.stream.Collectors;
import java.nio.file.Paths;
import java.util.Arrays;
import java.nio.file.Files;
import java.util.stream.Stream;
import java.io.IOException;
import java.text.DecimalFormat;

public class Covid
{
  // You can add your own variables between them.

  // You can add your own variables between them.

  // You must not change between them.
  private List<List<String>> rows;

  public Covid()
  {
    try
    {
      this.rows = Files
  				.lines(Paths.get("covid19.csv"))
  				.map(row -> Arrays.asList(row.split(",")))
  				.collect(Collectors.toList());
    }
    catch (IOException e)
    {
			e.printStackTrace();
		}
  }
  // You must not change between them.

  public void printOnlyCases(String location, String date)
  {
    int cases = this.rows
            .stream()
            .filter(e -> e.get(1).equals(location) && e.get(2).equals(date))
            .mapToInt(e -> Integer.parseInt(e.get(3))-Integer.parseInt(e.get(5)))
            .findFirst()
            .orElse(0);
    System.out.printf("Result: %s", cases);

  }

  public long getDateCount(String location)
  {
    long toReturn = this.rows
            .stream()
            .filter(e -> e.get(1).equals(location))
            .count();
    return toReturn;
  }

  public int getCaseSum(String date)
  {
    int toReturn = this.rows
            .stream()
            .filter(e -> e.get(2).equals(date))
            .mapToInt(e -> Integer.parseInt(e.get(4)))
            .sum();
    return toReturn;
  }

  public long getZeroRowsCount(String location)
  {
    long toReturn = this.rows
            .stream()
            .filter(e -> e.get(1).equals(location) &&
                         e.get(3).equals("0") && 
                         e.get(4).equals("0") && 
                         e.get(5).equals("0") && 
                         e.get(6).equals("0"))
            .count();
    return toReturn;
  }

  public double getAverageDeath(String location)
  {
    double toReturn = this.rows
            .stream()
            .filter(e -> e.get(1).equals(location))
            .mapToDouble(e -> Double.parseDouble(e.get(6)))
            .average()
            .orElse(0);

    toReturn = Double.parseDouble(new DecimalFormat("##.00").format(toReturn));
    return toReturn;
  }

  public String getFirstDeathDayInFirstTenRows(String location)
  {
    List<List<String>> firstTenRow = this.rows
            .stream()
            .filter(e -> e.get(1).equals(location))
            .limit(10)
            .collect(Collectors.toList());
    Predicate<List<String>> matchFunction =  e -> !e.get(6).equals("0");
    for(List<String> possibleMatch: firstTenRow) {
      if(matchFunction.test(possibleMatch)) {
        return(possibleMatch.get(2));
      }
    }
    return("Not Found");   
  }

  public String[] getDateCountOfAllLocations()
  {
    List<List<String>> uniqueLocations = this.rows.stream()
            .map(e -> e.subList(0,2))
            .distinct()
            .collect(Collectors.toList());
    String[] toReturn = uniqueLocations
            .stream()
            .map(e -> e.get(0) + ": " + getDateCount(e.get(1)))
            .toArray(String[]::new);
    return toReturn;
  }

  public List<String> getLocationsFirstDeathDay()
  {
    List<String> toReturn = this.rows
            .stream()
            .filter(e -> e.get(6).equals(e.get(5)) && !e.get(6).equals("0"))
            .map(e -> e.get(1) + ": " + e.get(2))
            .collect(Collectors.toList());
    return toReturn;
  }

  public String trimAndGetMax(String location, int trimCount)
  {
    List<List<String>> sortedRowsByLocation = this.rows
            .stream()
            .filter(e -> e.get(1).equals(location))
            .sorted((e1, e2)-> Integer.parseInt(e1.get(4)) - Integer.parseInt(e2.get(4)))
            .collect(Collectors.toList());
    String toReturn = sortedRowsByLocation
            .stream()
            .limit(sortedRowsByLocation.size()-trimCount)
            .skip(trimCount)
            .max((e1, e2)-> Integer.parseInt(e1.get(6)) - Integer.parseInt(e2.get(6)))
            .map(e -> e.get(2) + ": " + e.get(6))
            .get();
    return toReturn;
  }

  public List<List<String>> getOnlyCaseUpDays(String location)
  {
    List<List<String>> toReturn = this.rows
            .stream()
            .filter(e -> e.get(1).equals(location) && !e.get(4).equals("0"))
            .collect(Collectors.toList());
    System.out.printf("Result: %s", toReturn.size());

    return toReturn;
  }

  public static void main(String[] args)
  {
    
  }
}
