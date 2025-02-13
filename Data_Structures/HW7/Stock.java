/**
 * The Stock class represents a stock with a symbol, price, volume, and market capitalization.
 */
public class Stock {

  private String symbol;
  private double price;
  private long volume;
  private long marketCap;

  /**
   * Constructor to create a new Stock object with the given symbol, price, volume, and market capitalization.
   *
   * @param symbol the stock symbol
   * @param price the stock price
   * @param volume the stock volume
   * @param marketCap the stock's market capitalization
   */
  public Stock(String symbol, double price, long volume, long marketCap) {
    this.symbol = symbol;
    this.price = price;
    this.volume = volume;
    this.marketCap = marketCap;
  }

  /**
   * Gets the stock symbol.
   *
   * @return the stock symbol
   */
  public String getSymbol() {
    return symbol;
  }

  /**
   * Sets the stock symbol.
   *
   * @param symbol the new stock symbol
   */
  public void setSymbol(String symbol) {
    this.symbol = symbol;
  }

  /**
   * Gets the stock price.
   *
   * @return the stock price
   */
  public double getPrice() {
    return price;
  }

  /**
   * Sets the stock price.
   *
   * @param price the new stock price
   */
  public void setPrice(double price) {
    this.price = price;
  }

  /**
   * Gets the stock volume.
   *
   * @return the stock volume
   */
  public long getVolume() {
    return volume;
  }

  /**
   * Sets the stock volume.
   *
   * @param volume the new stock volume
   */
  public void setVolume(long volume) {
    this.volume = volume;
  }

  /**
   * Gets the stock's market capitalization.
   *
   * @return the stock's market capitalization
   */
  public long getMarketCap() {
    return marketCap;
  }

  /**
   * Sets the stock's market capitalization.
   *
   * @param marketCap the new market capitalization
   */
  public void setMarketCap(long marketCap) {
    this.marketCap = marketCap;
  }

  /**
   * Returns a string representation of the Stock object.
   *
   * @return a string representation of the Stock object
   */
  @Override
  public String toString() {
    return (
      "Stock [symbol=" +
      symbol +
      ", price=" +
      price +
      ", volume=" +
      volume +
      ", marketCap=" +
      marketCap +
      "]"
    );
  }
}
