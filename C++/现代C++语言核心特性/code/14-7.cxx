const char* ColorToString(Color c)
{
    switch (c)
    {
    using enum Color;
    case Red: return "Red";
    case Green: return "Green";
    case Blue: return "Blue";
    default:
        return "none";
    }
}