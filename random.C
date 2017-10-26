//
//  pinaki.cpp
//  
//
//  Created by Avinay Bhat on 10/1/17.
//

//some standard C++ includes
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <chrono>
#include <cmath>
#include <iterator>
#include <algorithm>
#include <fstream>

//some ROOT includes
#include "TInterpreter.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TFile.h"
#include "TMath.h"
#include "TTree.h"
#include "TBranch.h"
#include "TStyle.h"
#include "TCanvas.h"





void random()
{
    
    
    // create a random number generator
    gRandom = new TRandom3();
    
    // create a histogram
    TH1D * hist = new TH1D("data", ";x;N", 20, 0.0, 100.0);
    
    // fill in the histogram
    for (int i = 0; i < 100; ++i)
        hist->Fill(gRandom->Gaus(65.0, 5.0));
    
    TCanvas * c1= new TCanvas("c1", "random",5,5,800,600);
    hist->Draw();
    hist->SaveAs("random.eps");
}
