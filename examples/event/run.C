{
    gROOT->ProcessLine(".L CustomEvent.C++");
    WriteCustomEvents();
    ReadCustomEvents();
    gSystem->Exit(0);
}
