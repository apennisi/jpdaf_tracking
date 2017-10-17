//TODO INSERT NEW PARAMETERS TO READ

#include "tracker_param.h"

using namespace JPDAFTracker;

void TrackerParam::read(const std::string& filename)
{
  std::ifstream file;
  
  try
  {
    file.open(filename);
  }
  catch(...)
  {
    std::cerr << "Cannot open " << filename << std::endl;
    file.close();
    exit(-1);
  }
  
  if(!file.is_open())
  {
    std::cerr << "Error: file " << filename << " not found!" << std::endl;
    exit(-1);
  }
  
  
  std::string line;
  while(std::getline(file, line))
  {
      std::remove_if(line.begin(), line.end(), isspace);
      if(line.empty())
      {
	continue;
      }
      else if(line.find("[PD]") != std::string::npos)
      {
	std::getline(file, line);
	try
	{
	  pd = atof(line.c_str());
	}
	catch(...)
	{
	  std::cerr << "Error in converting the PD: " << line << std::endl;
	  exit(-1);
	}
      }
      else if(line.find("[PG]") != std::string::npos)
      {
	std::getline(file, line);
	try
	{
	  pg = atof(line.c_str());
	}
	catch(...)
	{
	  std::cerr << "Error in converting the PG: " << line << std::endl;
	  exit(-1);
	}
      }
      else if(line.find("[LOCAL_GSIGMA]") != std::string::npos)
      {
	std::getline(file, line);
	try
	{
	  g_sigma = atof(line.c_str());
	}
	catch(...)
	{
	  std::cerr << "Error in converting the LOCAL_GSIGMA: " << line << std::endl;
	  exit(-1);
	}
      }
      else if(line.find("[GLOBAL_GSIGMA]") != std::string::npos)
      {
	std::getline(file, line);
	try
	{
	  global_g_sigma = atof(line.c_str());
	}
	catch(...)
	{
	  std::cerr << "Error in converting the GLOBAL_GSIGMA: " << line << std::endl;
	  exit(-1);
	}
      }
      else if(line.find("[LAMBDA]") != std::string::npos)
      {
	std::getline(file, line);
	try
	{
	  lambda = atof(line.c_str());
	  gamma = lambda * 0.000001;
	}
	catch(...)
	{
	  std::cerr << "Error in converting the LAMBDA: " << line << std::endl;
	  exit(-1);
	}
      }
      else if(line.find("[GAMMA]") != std::string::npos)
      {
	std::getline(file, line);
	try
	{
	  std::istringstream iss(line);
	  float a, b;
	  if (!(iss >> a >> b)) { break; }
	  target_delta = cv::Point2f(a, b);
	}
	catch(...)
	{
	  std::cerr << "Error in converting the LAMBDA: " << line << std::endl;
	  exit(-1);
	}
      }
      else if(line.find("[R_MATRIX]") != std::string::npos)
      {
	std::getline(file, line);
	try
	{
	  std::istringstream iss(line);
	  float a, b, c, d;
	  if (!(iss >> a >> b)) { break; }
	  std::getline(file, line);
	  iss = std::istringstream(line);
	  if (!(iss >> c >> d)) { break; }
	  R << a, b, c, d;
	}
	catch(...)
	{
	  std::cerr << "Error in converting the R VALUES: " << line << std::endl;
	  exit(-1);
	}
      }
      else if(line.find("[LOCAL_ASSOCIATION_COST]") != std::string::npos)
      {
	std::getline(file, line);
	try
	{
	  assocCost = atof(line.c_str());
	}
	catch(...)
	{
	  std::cerr << "Error in converting the LOCAL_ASSOCIATION_COST: " << line << std::endl;
	  exit(-1);
	}
      }
      else if(line.find("[GLOBAL_ASSOCIATION_COST]") != std::string::npos)
      {
	std::getline(file, line);
	try
	{
	  global_assocCost = atof(line.c_str());
	}
	catch(...)
	{
	  std::cerr << "Error in converting the GLOBAL_ASSOCIATION_COST: " << line << std::endl;
	  exit(-1);
	}
      }
      else if(line.find("[DT]") != std::string::npos)
      {
	std::getline(file, line);
	try
	{
	  dt = atof(line.c_str());
	}
	catch(...)
	{
	  std::cerr << "Error in converting the DT: " << line << std::endl;
	  exit(-1);
	}
      }
      else if(line.find("[MAX_MISSED_RATE]") != std::string::npos)
      {
	std::getline(file, line);
	try
	{
	  max_missed_rate = atoi(line.c_str());
	}
	catch(...)
	{
	  std::cerr << "Error in converting the MAX_MISSED_RATE: " << line << std::endl;
	  exit(-1);
	}
      }
      else if(line.find("[MIN_ACCPETANCE_RATE]") != std::string::npos)
      {
	std::getline(file, line);
	try
	{
	  min_acceptance_rate = atoi(line.c_str());
	}
	catch(...)
	{
	  std::cerr << "Error in converting the MIN_ACCPETANCE_RATE: " << line << std::endl;
	  exit(-1);
	}
      }
      else
      {
	std::cerr << "Option: " << line << " does not exist!" << std::endl;
	exit(-1);
      } 
  }
 
  file.close();
  
  
  std::cout << "**************" << std::endl;
  std::cout << "****PARAMS****" << std::endl;
  std::cout << "**************" << std::endl;
  std::cout << "[PD]: " << pd << std::endl;
  std::cout << "[PG]: " << pg << std::endl;
  std::cout << "[LOCAL_GSIGMA]: " << g_sigma << std::endl;
  std::cout << "[GLOBAL_GSIGMA]: " << global_g_sigma << std::endl;
  std::cout << "[LAMBDA]: " << lambda << std::endl;
  std::cout << "[GAMMA]: " << gamma << std::endl;
  std::cout << "[R_MATRIX]; " << R << std::endl;
  std::cout << "[TARGET_DELTA]: " << target_delta.x << ", " << target_delta.y << std::endl;
  std::cout << "[DT]: " << dt << std::endl;
  std::cout << "[LOCAL_ASSOCIATION_COST]: " << assocCost << std::endl;
  std::cout << "[GLOBAL_ASSOCIATION_COST]: " << global_assocCost << std::endl;
  std::cout << "**************" << std::endl;
  
}
