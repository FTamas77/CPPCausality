# Test Data

## Sample Dataset

This directory contains a **sample dataset** for rapid development and testing:

- `KV-41762_202301_test.csv` - **~4,570 records** (subset of January 2023)
- Size: ~4.6 MB
- Purpose: Unit tests, integration tests, quick development

## Full Dataset (463K records)

For reproducing the paper results, use the **complete dataset** from the Python repository:

?? **[Vehicle Emissions Dataset (Python Repo)](https://github.com/FTamas77/Causality/blob/develop/datasets/vehicles/README.md)**

### Full Dataset Details:
- **463,568 records** total (Jan-Mar 2023)
- Files: `KV-41762_202301.xlsx` (~20 MB), `KV-41762_202302.xlsx` (~20 MB), `KV-41762_202303.xlsx` (~24 MB)
- Location: `Causality/datasets/vehicles/`

### To Run Benchmarks with Full Dataset:

1. **Download from Python repo:**
   ```sh
   # Option 1: Clone the Python repo
   git clone https://github.com/FTamas77/Causality.git
   cd Causality/datasets/vehicles/
   
   # Option 2: Direct download via GitHub
   # https://github.com/FTamas77/Causality/tree/develop/datasets/vehicles
   ```

2. **Convert XLSX to CSV** (if needed):
   ```python
   import pandas as pd
   df = pd.read_excel('KV-41762_202301.xlsx')
   df.to_csv('KV-41762_202301_full.csv', index=False)
   ```

3. **Update benchmark path:**
   ```cpp
   // In examples/benchmark_paper.cpp
   const std::string datasetFile = "path/to/KV-41762_202301_full.csv";
   ```

4. **Run benchmark:**
   ```sh
   cd build/Debug
   ./benchmark_paper
   ```

### Expected Performance (Full Dataset):

| Dataset Size | FCI Time | Speedup vs Python |
|-------------|----------|-------------------|
| **4.5K records** (test) | ~2-3s | N/A (too small) |
| **463K records** (full) | ~8-10s | **15× faster** |

---

**Note:** The sample dataset is sufficient for:
- ? Unit tests
- ? Integration tests
- ? Algorithm correctness verification
- ? Development iteration speed

For paper reproduction and performance benchmarks, use the full dataset.
