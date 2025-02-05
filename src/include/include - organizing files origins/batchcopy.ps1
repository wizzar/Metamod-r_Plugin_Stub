$destination = Get-Location  # Current folder
$fileList = Get-Content "files.txt"

foreach ($file in $fileList) {
    if ($file -eq "") {
        Write-Host "WARNING: Empty line found - Skipping."
        continue
    }

    if (Test-Path $file) {
        $parts = $file -split "\\"
        $cssdkIndex = [Array]::IndexOf($parts, "cssdk")

        if ($cssdkIndex -ge 0) {
            # Reconstruct the relative path from "cssdk" onward
            $relativePath = $parts[$cssdkIndex..($parts.Length - 2)] -join "\"
            $fileName = $parts[-1]  # Extract just the file name

            # Construct the correct target folder and file path
            $targetFolder = Join-Path $destination $relativePath
            $targetFile = Join-Path $targetFolder $fileName

            # Ensure the target directory exists
            if (!(Test-Path $targetFolder)) {
                New-Item -ItemType Directory -Path $targetFolder -Force | Out-Null
            }

            # Copy the file
            Copy-Item -Path $file -Destination $targetFile -Force
            Write-Host "Copied $file to $targetFile"
        } else {
            Write-Host "WARNING: 'cssdk' not found in path - Skipping: $file"
        }
    } else {
        Write-Host "WARNING: File not found - $file"
    }
}

Write-Host "Done!"
