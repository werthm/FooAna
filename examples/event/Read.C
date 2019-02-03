{
    gROOT->ProcessLine(".L CustomEvent.C++");
    ReadCustomEvents();
    gSystem->Exit(0);
}
