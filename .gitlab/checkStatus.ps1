$SEL = Select-String -Path $args[0] -Pattern $args[1]

if ($SEL -ne $null)
{
    $host.SetShouldExit(0)
    exit 0
}
else
{
    $host.SetShouldExit(1)
    exit 1
}