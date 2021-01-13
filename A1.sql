shabnoor sayed(ssayed10)
133649194
dbs311_203e31




Q4
DEFINE country_name = 'United Kingdom'
SELECT cust_no, cname, order_no, prod_name, SUM(ol.qty * ol.price) AS "Line Sales"
FROM countries co JOIN customers c
  ON co.country_id = c.country_cd
  JOIN orders o
    USING(cust_no)
    JOIN orderlines ol
      USING(order_no)
        JOIN products p
          USING(prod_no)
WHERE co.country_name = '&country_name'
  AND c.city LIKE 'L%'
  AND cust_no < 1000
GROUP BY cust_no, cname, order_no, prod_name
ORDER BY cust_no DESC;

Q5
SELECT CUST_NO, CNAME, COUNTRY_NAME, ORDER_DT
from
countries co join
customers c
on
c.COUNTRY_CD = co.COUNTRY_ID
join
orders o
using(CUST_NO)
WHERE (co.COUNTRY_NAME = 'Germany' OR co.COUNTRY_NAME = 'United Kingdom') AND o.order_dt like '%2014';

