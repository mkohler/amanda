# This file was automatically generated by SWIG (http://www.swig.org).
# Version 1.3.39
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

package Amanda::Debug;
use base qw(Exporter);
use base qw(DynaLoader);
package Amanda::Debugc;
bootstrap Amanda::Debug;
package Amanda::Debug;
@EXPORT = qw();

# ---------- BASE METHODS -------------

package Amanda::Debug;

sub TIEHASH {
    my ($classname,$obj) = @_;
    return bless $obj, $classname;
}

sub CLEAR { }

sub FIRSTKEY { }

sub NEXTKEY { }

sub FETCH {
    my ($self,$field) = @_;
    my $member_func = "swig_${field}_get";
    $self->$member_func();
}

sub STORE {
    my ($self,$field,$newval) = @_;
    my $member_func = "swig_${field}_set";
    $self->$member_func($newval);
}

sub this {
    my $ptr = shift;
    return tied(%$ptr);
}


# ------- FUNCTION WRAPPERS --------

package Amanda::Debug;

*debug_init = *Amanda::Debugc::debug_init;
*dbopen = *Amanda::Debugc::dbopen;
*dbreopen = *Amanda::Debugc::dbreopen;
*dbrename = *Amanda::Debugc::dbrename;
*dbclose = *Amanda::Debugc::dbclose;
*error = *Amanda::Debugc::error;
*critical = *Amanda::Debugc::critical;
*warning = *Amanda::Debugc::warning;
*message = *Amanda::Debugc::message;
*info = *Amanda::Debugc::info;
*debug = *Amanda::Debugc::debug;
*add_amanda_log_handler = *Amanda::Debugc::add_amanda_log_handler;
*suppress_error_traceback = *Amanda::Debugc::suppress_error_traceback;
*dbfd = *Amanda::Debugc::dbfd;
*dbfn = *Amanda::Debugc::dbfn;
*debug_dup_stderr_to_debug = *Amanda::Debugc::debug_dup_stderr_to_debug;

# ------- VARIABLE STUBS --------

package Amanda::Debug;

*error_exit_status = *Amanda::Debugc::error_exit_status;
*amanda_log_stderr = *Amanda::Debugc::amanda_log_stderr;
*amanda_log_syslog = *Amanda::Debugc::amanda_log_syslog;
*amanda_log_null = *Amanda::Debugc::amanda_log_null;

@EXPORT_OK = ();
%EXPORT_TAGS = ();


=head1 NAME

Amanda::Debug - support for debugging Amanda applications

=head1 SYNOPSIS

  use Amanda::Util qw( :constants );

  Amanda::Util::setup_application("amcooltool", "server", $CONTEXT_CMDLINE);

  debug("this is a debug message");
  die("Unable to frobnicate the ergonator");

See C<debug.h> for a more in-depth description of the logging
functionality of this module.

=head1 DEBUG LOGGING

Several debug logging functions, each taking a single string, are
available:

=over

=item C<error> - also aborts the program to produce a core dump

=item C<critical> - exits the program with C<$error_exit_status>

=item C<warning>

=item C<message>

=item C<info>

=item C<debug>

=back

Perl's built-in C<die> and C<warn> functions are patched to call
C<critical> and C<warning>, respectively.

All of the debug logging functions are available via the export tag
C<:logging>.

Applications can adjust the handling of log messages with
C<add_amanda_log_handler($hdlr)> where C<$hdlr> is a predefined log
destination.  The following destinations are available in this
package.  See L<Amanda::Logfile> for C<$amanda_log_trace_log>.

  $amanda_log_null
  $amanda_log_stderr
  $amanda_log_syslog

=head1 ADVANCED USAGE

Most applications should use L<Amanda::Util>'s C<setup_application> to
initialize the debug libraries.  The initialization functions
available from this module are thus considered "advanced", and the
reader is advised to consult the C header, C<debug.h>, for details.

Briefly, the functions C<dbopen> and C<dbrename> are used to open a
debug file whose pathname includes all of the relevant
information. C<dbclose> and C<dbreopen> are used to close that debug
file before transferring control to another process.

C<$error_exit_status> is the exit status with which C<critical> will
exit.

All of the initialization functions and variables are available via
the export tag C<:init>.

The current debug file's integer file descriptor (I<not> a Perl
filehandle) is available from C<dbfd()>.  Likewise, C<dbfn()> returns
the filename of the current debug file.

C<debug_dup_stderr_to_debug()> redirects, at the file-descriptor
level, C<STDERR> into the debug file.  This is useful when running
external applications which may produce error output.

=cut



push @EXPORT_OK, qw(debug_init dbopen dbreopen dbrename dbclose
    $error_exit_status);
push @{$EXPORT_TAGS{"init"}}, qw(debug_init dbopen dbreopen dbrename dbclose
    $error_exit_status);

sub _my_die {
    # $^S: (from perlvar)
    #  undef -> parsing module/eval
    #  1 -> executing an eval
    #  0 -> otherwise
    # we *only* want to call critical() in the "otherwise" case
    if (!defined($^S) or $^S == 1) {
	die(@_);
    } else {
	my ($msg) = @_;
	chomp $msg;
	suppress_error_traceback();
	critical(@_);
    }
};
$SIG{__DIE__} = \&_my_die;

sub _my_warn {
    my ($msg) = @_;
    chomp $msg;
    warning(@_);
};
$SIG{__WARN__} = \&_my_warn;

# utility function for test scripts, which want to use the regular
# perl mechanisms
sub disable_die_override {
    delete $SIG{__DIE__};
    delete $SIG{__WARN__};
}

push @EXPORT_OK, qw(error critical warning message info debug);
push @{$EXPORT_TAGS{"logging"}}, qw(error critical warning message info debug);

push @EXPORT_OK, qw(add_amanda_log_handler
    $amanda_log_stderr $amanda_log_syslog $amanda_log_null);
push @{$EXPORT_TAGS{"logging"}}, qw(add_amanda_log_handler
    $amanda_log_stderr $amanda_log_syslog $amanda_log_null);
1;
