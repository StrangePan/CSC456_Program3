#include "msim.h"

int msim(int argc, char*argv[])
{
  ifstream fin;
  vector<int> ref_string;
  int reference;
  int alg;
  int num_frames;

  // Display help if no arguments are received
  if (argc < 4)
  {
    cout << "Usage:\n\tmsim <file> <frames> <fifo|opt|lru|lfu|sc|c>" << endl;
    return 0;
  }
  
  // Attempt to open file
  fin.open(argv[1]);
  if (!fin)
  {
    cout << "Failed to open " << argv[1] << " for input" << endl;
    return 1;
  }
  
  // Parse memory sizes
  num_frames = (int) strtol(argv[2], NULL, 10);
  
  // Make sure number of frames is positive integer
  if (num_frames < 1)
  {
    cout << "Invalid number of frames " << num_frames;
    cout << ": expected positive integer" << endl;
    return 1;
  }
  
  // Parse algorithm argument
  if (strcmp(argv[3], "fifo") == 0)
    alg = 0;
  else if (strcmp(argv[3], "opt") == 0)
    alg = 1;
  else if (strcmp(argv[3], "lru") == 0)
    alg = 2;
  else if (strcmp(argv[3], "lfu") == 0)
    alg = 3;
  else if (strcmp(argv[3], "sc") == 0)
    alg = 4;
  else if (strcmp(argv[3], "c") == 0)
    alg = 5;
  else
  {
    cout << "Unknown page replacement algorithm " << argv[2] << endl;
    alg = -1;
    return 1;
  }
  
  // Read in values from file
  while (fin >> reference)
  {
    ref_string.push_back(reference);
  }

  // Call appropriate algorithm function
  switch (alg)
  {
  case 0:
    msim_fifo(ref_string, (unsigned int) num_frames);
    break;
    
  case 1:
    // TODO Optimal
    break;
    
  case 2:
    // TODO LRU
    break;
    
  case 3:
    // TODO LFU
    break;
    
  case 4:
    // TODO Second Chance
    break;
    
  case 5:
    // TODO Clock
    break;
  }

  return 0;
}

void msim_fifo(vector<int>& ref_string, unsigned int num_frames)
{
  vector<int> frames;
  auto frame = frames.end();
  int faults = 0;
  int padding = 0;
  int temp;
  bool fault;
  unsigned int i;
  unsigned int j;
  
  // For formatting reasons, find 'widest' number
  for (i = 0; i < ref_string.size(); i++)
  {
    // Handle positive and negative numbers differently
    if (ref_string[i] < 0)
    {
      temp = (int) (ceil(log10(-ref_string[i] + 1))) + 1;
    }
    else
    {
      temp = (int) (ceil(log10(ref_string[i] + 1)));
    }
    
    if (temp > padding) padding = temp;
  }
  
  cout << right;
  
  // Work through reference string
  for (i = 0; i < ref_string.size(); i++)
  {
    frame = find(frames.begin(), frames.end(), ref_string[i]);
    fault = (frame == frames.end());
    
    // Check for page faults
    if (fault)
    {
      // Handle page faults
      faults++;
    }
    else
    {
      // Remove item from queue so we can insert it at beginning again
      frames.erase(frame);
    }
    
    // Add frame to front
    frames.insert(frames.begin(), ref_string[i]);
    
    // Delete last item if overflow
    while (frames.size() > num_frames)
    {
      frames.erase(frames.end()-1);
    }
    
    // Output frame state
    cout << setw(padding) << ref_string[i] << " -> ";
    for (j = 0; j < num_frames; j++)
    {
      cout << "| ";
      if (j < frames.size())
      {
        cout << setw(padding) << frames[j] << " ";
      }
      else
      {
        cout << setw(padding) << " " << " ";
      }
    }
    cout << "|";
    if (fault)
    {
      cout << " FAULT";
    }
    cout << endl;
  }
  
  // Final output
  cout << "\n";
  cout << "page faults: " << faults << endl;
}

