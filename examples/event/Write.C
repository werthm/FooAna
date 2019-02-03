{
    gROOT->ProcessLine(".L CustomEvent.C++");
    WriteCustomEvents();
    gSystem->Exit(0);
}
