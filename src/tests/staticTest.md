# Test Case Description for `Statistic::testConditionalIndependence` Method

## Test Case 1: Conditional Independence with Conditioning Set

**Objective:**  
To verify that the `testConditionalIndependence` method correctly calculates the p-value for conditional independence between two columns, given a non-empty conditioning set.

**Preconditions:**  
- A dataset with three columns of numerical data.

**Input Data:**  
- Data: `{{1, 2.1, 3.2, 4.3, 5.4}, {2, 4.1, 6.2, 8.3, 10.4}, {1.5, 2.1, 1.6, 2.2, 1.7}}`
- Columns to test for independence: `0` and `1`
- Conditioning set: `{2}`

**Execution Steps:**
1. Create a `Dataset` object with the given data.
2. Instantiate a `Statistic` object.
3. Call the `testConditionalIndependence` method with the dataset, columns `0` and `1`, and the conditioning set `{2}`.

**Expected Results:**  
- The p-value should be within the range `[0, 1]`.

## Test Case 2: Independence Without Conditioning Set

**Objective:**  
To verify that the `testConditionalIndependence` method correctly calculates the p-value for independence between two columns without any conditioning set.

**Preconditions:**  
- A dataset with three columns of numerical data.

**Input Data:**  
- Data: `{{1, 2.01, 3.02}, {2, 4.01, 6.02}, {1.1, 2.02, 3.04}}`
- Columns to test for independence: `0` and `1`
- Conditioning set: `{}`

**Execution Steps:**
1. Create a `Dataset` object with the given data.
2. Instantiate a `Statistic` object.
3. Call the `testConditionalIndependence` method with the dataset, columns `0` and `1`, and an empty conditioning set.

**Expected Results:**  
- The p-value should be within the range `[0, 1]`.
- The p-value should be greater than `0.05`, indicating no significant dependence.

## Test Case 3: Constant Variable

**Objective:**  
To verify that the `testConditionalIndependence` method correctly handles cases where one of the variables is constant.

**Preconditions:**  
- A dataset with three columns of numerical data where one column is constant.

**Input Data:**  
- Data: `{{1, 1, 1, 1, 1}, {2, 3, 4, 5, 6}, {1.1, 2.1, 3.1, 4.1, 5.1}}`
- Columns to test for independence: `0` and `1`
- Conditioning set: `{}`

**Execution Steps:**
1. Create a `Dataset` object with the given data.
2. Instantiate a `Statistic` object.
3. Call the `testConditionalIndependence` method with the dataset, columns `0` and `1`, and an empty conditioning set.

**Expected Results:**  
- The function should handle the constant variable gracefully.
- The p-value should be within the range `[0, 1]`.

## Test Case 4: Conditional Independence with Residuals Correlation

**Objective:**  
To verify that the `testConditionalIndependence` method correctly calculates the p-value when residuals correlation is close to ±1.

**Preconditions:**  
- A dataset with three columns of numerical data.

**Input Data:**  
- Data: `{{1, 2, 3, 4, 5}, {2, 4, 6, 8, 10}, {3, 4, 5, 6, 7}}`
- Columns to test for independence: `0` and `1`
- Conditioning set: `{2}`

**Execution Steps:**
1. Create a `Dataset` object with the given data.
2. Instantiate a `Statistic` object.
3. Call the `testConditionalIndependence` method with the dataset, columns `0` and `1`, and the conditioning set `{2}`.

**Expected Results:**  
- The p-value should be within the range `[0, 1]`.

## Test Case 5: Independence with Non-Linear Relationship

**Objective:**  
To verify that the `testConditionalIndependence` method correctly calculates the p-value for independence between two columns with a non-linear relationship.

**Preconditions:**  
- A dataset with three columns of numerical data.

**Input Data:**  
- Data: `{{1, 2, 3, 4, 5}, {5, 4, 3, 2, 1}, {1, 2, 3, 4, 5}}`
- Columns to test for independence: `0` and `1`
- Conditioning set: `{2}`

**Execution Steps:**
1. Create a `Dataset` object with the given data.
2. Instantiate a `Statistic` object.
3. Call the `testConditionalIndependence` method with the dataset, columns `0` and `1`, and the conditioning set `{2}`.

**Expected Results:**  
- The p-value should be within the range `[0, 1]`.
