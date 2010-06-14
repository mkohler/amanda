# This file was automatically generated by SWIG (http://www.swig.org).
# Version 1.3.39
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

package Amanda::Tests;
use base qw(Exporter);
use base qw(DynaLoader);
package Amanda::Testsc;
bootstrap Amanda::Tests;
package Amanda::Tests;
@EXPORT = qw();

# ---------- BASE METHODS -------------

package Amanda::Tests;

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

package Amanda::Tests;

*take_guint64 = *Amanda::Testsc::take_guint64;
*take_gint64 = *Amanda::Testsc::take_gint64;
*take_guint32 = *Amanda::Testsc::take_guint32;
*take_gint32 = *Amanda::Testsc::take_gint32;
*take_guint16 = *Amanda::Testsc::take_guint16;
*take_gint16 = *Amanda::Testsc::take_gint16;
*take_guint8 = *Amanda::Testsc::take_guint8;
*take_gint8 = *Amanda::Testsc::take_gint8;
*give_guint64 = *Amanda::Testsc::give_guint64;
*give_gint64 = *Amanda::Testsc::give_gint64;
*give_guint32 = *Amanda::Testsc::give_guint32;
*give_gint32 = *Amanda::Testsc::give_gint32;
*give_guint16 = *Amanda::Testsc::give_guint16;
*give_gint16 = *Amanda::Testsc::give_gint16;
*give_guint8 = *Amanda::Testsc::give_guint8;
*give_gint8 = *Amanda::Testsc::give_gint8;
*sizeof_size_t = *Amanda::Testsc::sizeof_size_t;
*write_random_file = *Amanda::Testsc::write_random_file;
*verify_random_file = *Amanda::Testsc::verify_random_file;
*try_threads = *Amanda::Testsc::try_threads;

# ------- VARIABLE STUBS --------

package Amanda::Tests;


@EXPORT_OK = ();
%EXPORT_TAGS = ();


=head1 NAME

Amanda::Tests -- test functions for installchecks

=head1 SYNOPSIS

This module exists only to provide functions for installcheck scripts
to call, mostly to test that various C-Perl interface techniques are
working.

=cut


1;
